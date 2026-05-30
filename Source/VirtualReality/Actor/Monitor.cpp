#include "Monitor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Actor/CCTV.h"
#include "EngineUtils.h"
#include "VirtualReality.h"
#include "Components/RectLightComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/VRPlayer.h"
#include "Subsystem/SoundSubsystem.h"

AMonitor::AMonitor()
{
	MonitorRoot = CreateDefaultSubobject<USceneComponent>("MonitorRoot");
	SetRootComponent(MonitorRoot);
	
	MonitorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MonitorMesh"));
	MonitorMesh->SetupAttachment(MonitorRoot);
	MonitorMesh->SetRelativeScale3D(FVector(1.0f, 2.5f, 2.5f));
	
	ScreenMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScreenMesh"));
	ScreenMesh->SetupAttachment(MonitorMesh);
	ScreenMesh->SetRelativeLocation(FVector(4.0f, 0.0f, 31.6f));
	ScreenMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	ScreenMesh->SetRelativeScale3D(FVector(0.6f, 0.001f, 0.14f));
	
	ScreenRectLight = CreateDefaultSubobject<URectLightComponent>(TEXT("ScreenLight"));
	ScreenRectLight->SetupAttachment(ScreenMesh);
	ScreenRectLight->SetIntensityUnits(ELightUnits::Unitless);
	ScreenRectLight->SetVisibility(false);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Monitor(TEXT("/Game/AtmosphericHouse/Meshes/Meshes_props/Electronics/SM_Computer_screen"));
	if (SM_Monitor.Succeeded())
	{
		MonitorMesh->SetStaticMesh(SM_Monitor.Object);
	}
}

void AMonitor::BeginPlay()
{
	Super::BeginPlay();

	// 화면용 다이나믹 머티리얼 인스턴스를 생성합니다.
	if (ScreenMaterial)
	{
		ScreenMaterialInstance = UMaterialInstanceDynamic::Create(ScreenMaterial, this);
		ScreenMesh->SetMaterial(0, ScreenMaterialInstance);
	}

	ScreenRectLight->SetVisibility(true);

	StartCCTVNoise();
}

void AMonitor::SetActiveCCTV(bool bIsEnable)
{
	if (RegisteredCCTVs.IsValidIndex(ActiveCCTVIndex))
	{
		RegisteredCCTVs[ActiveCCTVIndex]->SetCaptureEnabled(bIsEnable);
	}
}

void AMonitor::DeactivateAllCCTVs()
{
	for (TObjectPtr<ACCTV>& CCTV : RegisteredCCTVs)
	{
		if (CCTV)
		{
			CCTV->SetCaptureEnabled(false);
		}
	}

	ActiveCCTVIndex = 0;
}

void AMonitor::SwitchToNextCCTV()
{
	if (RegisteredCCTVs.IsEmpty() || bIsScanning) return;

	// 현재 CCTV의 캡쳐를 비활성화합니다.
	SetActiveCCTV(false);
	SetScreenMaterial(5.f, 0.1f, BlackRenderTarget);

	ChannelSwitchSoundComponent = USoundSubsystem::Get(this).PlaySFXAtLocationByName(TEXT("ChannelSwitch"), GetActorLocation());
	
	// 다음으로 활성화할 CCTV의 인덱스를 저장합니다.
	ActiveCCTVIndex = (ActiveCCTVIndex + 1) % RegisteredCCTVs.Num();
	
	// 다음 CCTV의 캡쳐를 미리 활성화합니다.
	SetActiveCCTV(true);

	// 일정 시간 후 실제 CCTV 전환을 수행합니다.
	GetWorldTimerManager().SetTimer(SwitchTimerHandle, this, &AMonitor::ApplyNextCCTV, NoiseEffectDuration, false);
	
	// 채널 전환 델리게이트를 호출합니다.
	// EventManager가 다음 CCTV의 이벤트를 발생시키는 것을 비활성화합니다.
	if (OnMonitorChangedDelegate.IsBound())
	{
		OnMonitorChangedDelegate.Broadcast(RegisteredCCTVs[ActiveCCTVIndex]);
	}
}

void AMonitor::OnLeverReachedEnd()
{
	bIsScanning = true;
	ScreenMesh->SetMaterial(0, ScanningMaterial);

	ScanningSoundComponent = USoundSubsystem::Get(this).PlaySFXAtLocationByName(TEXT("Scanning"), GetActorLocation());

	// ScanningDuration 후 원래 머티리얼로 복원합니다.
	GetWorldTimerManager().SetTimer(ScanningTimerHandle, this, &AMonitor::RestoreScreenMaterial, ScanningDuration, false);
}

void AMonitor::RestoreScreenMaterial()
{
	bIsScanning = false;
	ScreenMesh->SetMaterial(0, ScreenMaterialInstance);

	if (ScanningSoundComponent)
	{
		ScanningSoundComponent->Stop();
		ScanningSoundComponent = nullptr;
	}
}

void AMonitor::ApplyNextCCTV()
{
	// 다음 CCTV가 캡쳐하는 텍스처를 모니터에 적용합니다.
	UTextureRenderTarget2D* RenderTarget = RegisteredCCTVs[ActiveCCTVIndex]->GetRenderTarget();
	SetScreenMaterial(0.f, 0.f, RenderTarget);

	if (ChannelSwitchSoundComponent)
	{
		ChannelSwitchSoundComponent->Stop();
		ChannelSwitchSoundComponent = nullptr;
	}
}

void AMonitor::StartCCTVNoise()
{
	CCTVNoiseSoundComponent = USoundSubsystem::Get(this).PlaySFXAtLocationByName(TEXT("CCTVNoise"), MonitorMesh->GetComponentLocation());
}

void AMonitor::HandlePlayerDeath()
{
	if (ChannelSwitchSoundComponent)
	{
		ChannelSwitchSoundComponent->Stop();
		ChannelSwitchSoundComponent = nullptr;
	}

	if (ScanningSoundComponent)
	{
		ScanningSoundComponent->Stop();
		ScanningSoundComponent = nullptr;
	}

	if (CCTVNoiseSoundComponent)
	{
		CCTVNoiseSoundComponent->Stop();
		CCTVNoiseSoundComponent = nullptr;
	}

	// 진행 중인 채널 전환/스캐닝 타이머를 정지하여 노이즈 화면이 다른 상태로 복원되지 않도록 합니다.
	GetWorldTimerManager().ClearTimer(SwitchTimerHandle);
	GetWorldTimerManager().ClearTimer(ScanningTimerHandle);

	// 모든 CCTV 캡쳐를 중단합니다.
	DeactivateAllCCTVs();

	// 스캐닝 머티리얼이 적용되어 있을 수 있으므로 다이나믹 머티리얼로 되돌립니다.
	bIsScanning = false;
	if (ScreenMaterialInstance)
	{
		ScreenMesh->SetMaterial(0, ScreenMaterialInstance);
	}

	// 화면을 노이즈 상태로 고정합니다.
	SetScreenMaterial(5.f, 0.1f, BlackRenderTarget);
}

void AMonitor::SetScreenMaterial(const float InNoisePower, const float InNoiseIntensity, UTextureRenderTarget2D* InRenderTarget)
{
	ScreenMaterialInstance->SetScalarParameterValue(NoisePowerParameterName, InNoisePower);
	ScreenMaterialInstance->SetScalarParameterValue(NoiseIntensityParameterName, InNoiseIntensity);
	ScreenMaterialInstance->SetTextureParameterValue(RenderTargetParameterName, InRenderTarget);
}

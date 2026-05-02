#include "Monitor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Actor/CCTV.h"
#include "EngineUtils.h"
#include "VirtualReality.h"
#include "Components/RectLightComponent.h"
#include "Components/AudioComponent.h"
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

	if (USoundSubsystem* SoundSubsystem = GetWorld()->GetSubsystem<USoundSubsystem>())
	{
		SoundSubsystem->PlayAtLocation(ChannelSwitchSound, GetActorLocation());
	}
	
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

	if (USoundSubsystem* SoundSubsystem = GetWorld()->GetSubsystem<USoundSubsystem>())
	{
		ScanningSoundComponent = SoundSubsystem->PlayAtLocation(ScanningSound, GetActorLocation());
	}

	// ScanningDuration 후 원래 머티리얼로 복원합니다.
	GetWorldTimerManager().SetTimer(ScanningTimerHandle, this, &AMonitor::RestoreScreenMaterial, ScanningDuration, false);
}

void AMonitor::RestoreScreenMaterial()
{
	bIsScanning = false;
	ScreenMesh->SetMaterial(0, ScreenMaterialInstance);

	if (USoundSubsystem* SoundSubsystem = GetWorld()->GetSubsystem<USoundSubsystem>())
	{
		SoundSubsystem->Stop(ScanningSoundComponent);
		ScanningSoundComponent = nullptr;
	}
}

void AMonitor::ApplyNextCCTV()
{
	// 다음 CCTV가 캡쳐하는 텍스처를 모니터에 적용합니다.
	UTextureRenderTarget2D* RenderTarget = RegisteredCCTVs[ActiveCCTVIndex]->GetRenderTarget();
	SetScreenMaterial(0.f, 0.f, RenderTarget);

	if (USoundSubsystem* SoundSubsystem = GetWorld()->GetSubsystem<USoundSubsystem>())
	{
		SoundSubsystem->PlayAtLocation(CCTVNoiseSound, GetActorLocation());
	}
}

void AMonitor::SetScreenMaterial(const float InNoisePower, const float InNoiseIntensity, UTextureRenderTarget2D* InRenderTarget)
{
	ScreenMaterialInstance->SetScalarParameterValue(NoisePowerParameterName, InNoisePower);
	ScreenMaterialInstance->SetScalarParameterValue(NoiseIntensityParameterName, InNoiseIntensity);
	ScreenMaterialInstance->SetTextureParameterValue(RenderTargetParameterName, InRenderTarget);
}

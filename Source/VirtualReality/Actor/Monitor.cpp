#include "Monitor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Actor/CCTV.h"
#include "EngineUtils.h"
#include "VirtualReality.h"
#include "Components/RectLightComponent.h"

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

	// 월드 내 모든 CCTV를 수집합니다.
	CollectCCTVs();
}

void AMonitor::CollectCCTVs()
{
	RegisteredCCTVs.Reset();

	for (ACCTV* CCTV : TActorRange<ACCTV>(GetWorld()))
	{
		RegisteredCCTVs.Emplace(CCTV);
	}

	LOG(TEXT("CCTV %d개가 수집되었습니다."), RegisteredCCTVs.Num());
}

void AMonitor::SetActiveCCTV(int32 Index)
{
	// 이전 활성 CCTV를 비활성화합니다.
	if (RegisteredCCTVs.IsValidIndex(ActiveCCTVIndex))
	{
		RegisteredCCTVs[ActiveCCTVIndex]->SetCaptureEnabled(false);
	}

	// 새 CCTV를 활성화합니다.
	ActiveCCTVIndex = Index;
	RegisteredCCTVs[ActiveCCTVIndex]->SetCaptureEnabled(true);

	LOG(TEXT("활성 CCTV가 인덱스 %d로 변경되었습니다."), ActiveCCTVIndex);
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

	ActiveCCTVIndex = -1;

	LOG(TEXT("모든 CCTV가 비활성화되었습니다."));
}

void AMonitor::SwitchToNextCCTV()
{
	if (RegisteredCCTVs.IsEmpty()) return;

	ScreenRectLight->SetVisibility(true);

	// 다음 인덱스를 미리 계산하여 저장합니다.
	PendingCCTVIndex = (ActiveCCTVIndex + 1) % RegisteredCCTVs.Num();

	// 노이즈 파라미터를 10으로 설정하여 전환 효과를 연출합니다.
	if (ScreenMaterialInstance)
	{
		ScreenMaterialInstance->SetScalarParameterValue(NoiseParameterName, 10.f);
		ScreenMaterialInstance->SetTextureParameterValue(RenderTargetParameterName, BlackRenderTarget);
	}

	// 일정 시간 후 실제 CCTV 전환을 수행합니다.
	GetWorldTimerManager().SetTimer(SwitchTimerHandle, this, &AMonitor::ApplyNextCCTV, NoiseEffectDuration, false);
}

void AMonitor::ApplyNextCCTV()
{
	// 노이즈 파라미터를 원래 값으로 복원합니다.
	if (ScreenMaterialInstance)
	{
		ScreenMaterialInstance->SetScalarParameterValue(NoiseParameterName, 3.f);
	}

	// 활성 CCTV를 전환합니다.
	SetActiveCCTV(PendingCCTVIndex);

	// 전환된 CCTV의 렌더 타겟을 화면 머티리얼에 적용합니다.
	if (UTextureRenderTarget2D* RenderTarget = RegisteredCCTVs[PendingCCTVIndex]->GetRenderTarget())
	{
		ScreenMaterialInstance->SetTextureParameterValue(RenderTargetParameterName, RenderTarget);
	}
	
	if (OnMonitorChangedDelegate.IsBound())
	{
		OnMonitorChangedDelegate.Broadcast();
	}

	LOG(TEXT("모니터가 CCTV 인덱스 %d로 전환되었습니다."), PendingCCTVIndex);
}
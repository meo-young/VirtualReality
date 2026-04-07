#include "Monitor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "EnhancedInputComponent.h"
#include "Manager/CCTVManager.h"
#include "Actor/CCTV.h"
#include "VirtualReality.h"
#include "Components/PointLightComponent.h"

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
	
	ScreenLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("ScreenLight"));
	ScreenLight->SetupAttachment(ScreenMesh);
	ScreenLight->SetIntensityUnits(ELightUnits::Unitless);
	ScreenLight->SetVisibility(false);
	
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
}

void AMonitor::SwitchToCCTV(int32 Index)
{
	ScreenLight->SetVisibility(true);

	UCCTVManager* CCTVManager = GetWorld()->GetSubsystem<UCCTVManager>();
	if (!CCTVManager) return;

	const TArray<TObjectPtr<ACCTV>>& CCTVs = CCTVManager->GetRegisteredCCTVs();
	if (CCTVs.IsEmpty()) return;

	// 인덱스를 CCTV 배열 범위 내에서 순환시킵니다.
	const int32 ClampedIndex = (Index % CCTVs.Num() + CCTVs.Num()) % CCTVs.Num();

	// CCTVManager에 활성 CCTV 전환을 요청합니다.
	CCTVManager->SetActiveCCTV(ClampedIndex);
	CurrentCCTVIndex = ClampedIndex;

	// 전환된 CCTV의 렌더 타겟을 화면 머티리얼에 적용합니다.
	if (ScreenMaterialInstance)
	{
		USceneCaptureComponent2D* CaptureComp = CCTVs[ClampedIndex]->GetSceneCaptureComponent();
		if (CaptureComp && CaptureComp->TextureTarget)
		{
			ScreenMaterialInstance->SetTextureParameterValue(RenderTargetParameterName, CaptureComp->TextureTarget);
		}
	}

	LOG(TEXT("모니터가 CCTV 인덱스 %d로 전환되었습니다."), ClampedIndex);
}

void AMonitor::OnNextCCTV()
{
	// 다음 인덱스로 전환합니다. SwitchToCCTV 내부에서 순환 처리됩니다.
	SwitchToCCTV(CurrentCCTVIndex + 1);
}

void AMonitor::OnPrevCCTV()
{
	// 이전 인덱스로 전환합니다. SwitchToCCTV 내부에서 순환 처리됩니다.
	SwitchToCCTV(CurrentCCTVIndex - 1);
}
#include "CCTV.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Manager/CCTVManager.h"
#include "VirtualReality.h"

ACCTV::ACCTV()
{
	FakeCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FakeCameraComponent"));
	FakeCameraComponent->SetupAttachment(GetRootComponent());

	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	SceneCaptureComponent->SetupAttachment(FakeCameraComponent);

	// 기본적으로 캡처를 비활성화하여 불필요한 렌더링 비용을 절약합니다.
	SceneCaptureComponent->bCaptureEveryFrame = false;
	SceneCaptureComponent->bCaptureOnMovement = false;
}

void ACCTV::BeginPlay()
{
	Super::BeginPlay();

	// CCTVManager에 자신을 등록합니다.
	if (UCCTVManager* CCTVManager = GetWorld()->GetSubsystem<UCCTVManager>())
	{
		CCTVManager->RegisterCCTV(this);
	}
}

void ACCTV::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// CCTVManager에서 자신을 해제합니다.
	if (UCCTVManager* CCTVManager = GetWorld()->GetSubsystem<UCCTVManager>())
	{
		CCTVManager->UnregisterCCTV(this);
	}

	Super::EndPlay(EndPlayReason);
}

void ACCTV::SetCaptureEnabled(bool bEnabled)
{
	if (!SceneCaptureComponent) return;

	// 활성화 여부에 따라 매 프레임 캡처 설정을 변경합니다.
	SceneCaptureComponent->SetActive(bEnabled);
	SceneCaptureComponent->bCaptureEveryFrame = bEnabled;
	SceneCaptureComponent->bCaptureOnMovement = bEnabled;

	LOG(TEXT("CCTV 캡처 상태가 [%s]로 변경되었습니다."), bEnabled ? TEXT("활성화") : TEXT("비활성화"));
}
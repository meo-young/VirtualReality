#include "CCTV.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
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
	SceneCaptureComponent->ShowFlags.SetMotionBlur(false);
	SceneCaptureComponent->ShowFlags.SetEyeAdaptation(false);
	SceneCaptureComponent->ShowFlags.SetLocalExposure(false);
	SceneCaptureComponent->ShowFlags.SetPostProcessMaterial(false);
	SceneCaptureComponent->ShowFlags.SetToneCurve(false);
	SceneCaptureComponent->ShowFlags.SetTonemapper(false);
	SceneCaptureComponent->ShowFlags.SetBloom(false);
	SceneCaptureComponent->ShowFlags.SetAmbientOcclusion(false);
	SceneCaptureComponent->ShowFlags.SetDynamicShadows(false);
	SceneCaptureComponent->ShowFlags.SetFog(false);
	SceneCaptureComponent->ShowFlags.SetLensFlares(false);
	SceneCaptureComponent->ShowFlags.SetAntiAliasing(false);
	SceneCaptureComponent->ShowFlags.SetDepthOfField(false);
	SceneCaptureComponent->LODDistanceFactor = 10.0f;
	SceneCaptureComponent->SetActive(false);
}

void ACCTV::BeginPlay()
{
	Super::BeginPlay();
	
	SceneCaptureComponent->CaptureScene();
}

void ACCTV::SetCaptureEnabled(bool bEnabled)
{
	SceneCaptureComponent->bCaptureEveryFrame = bEnabled;
	SceneCaptureComponent->SetActive(bEnabled);
}

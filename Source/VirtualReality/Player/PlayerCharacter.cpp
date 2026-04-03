#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "InputActionValue.h"
#include "MotionControllerComponent.h"
#include "VirtualReality.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "InputMappingContext.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_Player(TEXT("/Game/_VirtualReality/Input/IMC_Player"));
	{
		if (IMC_Player.Succeeded())
		{
			DefaultInputMappingContext = IMC_Player.Object;
		}
	}

	// 액터가 컨트롤러 Yaw를 따라 회전하도록 활성화합니다.
	bUseControllerRotationYaw = true;

	// VR 트래킹 원점 (발 기준, 캡슐에 부착)
	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VROrigin"));
	VROrigin->SetupAttachment(GetRootComponent());

	// HMD 카메라 (VRRoot에 부착)
	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(VROrigin);

	// 왼손 컨트롤러 (VRRoot에 부착)
	LeftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftController"));
	LeftController->SetupAttachment(VROrigin);
	LeftController->SetTrackingMotionSource(FName("Left"));
	
	// 왼쪽 손 메쉬
	LeftHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandMesh"));
	LeftHandMesh->SetupAttachment(LeftController);

	// 오른손 컨트롤러 (VRRoot에 부착)
	RightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightController"));
	RightController->SetupAttachment(VROrigin);
	RightController->SetTrackingMotionSource(FName("Right"));
	
	// 오른쪽 손 메쉬 
	RightHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandMesh"));
	RightHandMesh->SetupAttachment(RightController);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//@TODO : PC 버전과 VR 버전에서 입력 액션을 분리하는 것을 고려해 볼 수 있습니다.
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InitVRSetting();
}

void APlayerCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	
	if (!DefaultInputMappingContext)
	{
		LOG(TEXT("DefaultInputMappingContext가 설정되지 않았습니다. 블루프린트에서 할당해 주세요."));
		return;
	}
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))	
	{
		if (ULocalPlayer* LP = PlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				Subsystem->AddMappingContext(DefaultInputMappingContext, 0);
			}
		}
	}
}

void APlayerCharacter::InitVRSetting()
{
	// HMD가 연결되어 있으면 발 기준 트래킹 원점으로 설정합니다.
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::LocalFloor);
		LOG(TEXT("HMD 감지 — 트래킹 원점을 Floor로 설정했습니다."));
	}
	else
	{
		LOG(TEXT("HMD가 감지되지 않았습니다. 일반 카메라 모드로 실행됩니다."));
	}
}

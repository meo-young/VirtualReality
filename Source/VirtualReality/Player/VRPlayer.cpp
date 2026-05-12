#include "VRPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "VirtualReality.h"
#include "VRHand.h"
#include "Animation/VRHandAnimInstance.h"
#include "Camera/CameraComponent.h"

AVRPlayer::AVRPlayer()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_Default(TEXT("/Game/VRTemplate/Input/IMC_Default"));
	if (IMC_Default.Succeeded())
	{
		DefaultMappingContext = IMC_Default.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_Hands(TEXT("/Game/VRTemplate/Input/IMC_Hands"));
	if (IMC_Hands.Succeeded())
	{
		HandsMappingContext = IMC_Hands.Object;
	}
	
	static ConstructorHelpers::FClassFinder<AVRHand> BP_LeftHand(TEXT("/Game/_VirtualReality/Blueprint/Player/BP_LeftHand"));
	if (BP_LeftHand.Succeeded())
	{
		LeftHandClass = BP_LeftHand.Class;
	}

	static ConstructorHelpers::FClassFinder<AVRHand> BP_RightHand(TEXT("/Game/_VirtualReality/Blueprint/Player/BP_RightHand"));
	if (BP_RightHand.Succeeded())
	{
		RightHandClass = BP_RightHand.Class;
	}
	
	VROrigin = CreateDefaultSubobject<USceneComponent>("VROrigin");
	VROrigin->SetupAttachment(GetMesh());
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(VROrigin);
}

void AVRPlayer::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* EnhancedInput = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				EnhancedInput->AddMappingContext(DefaultMappingContext, 0);
				EnhancedInput->AddMappingContext(HandsMappingContext, 0);
			}
		}
	}
}

void AVRPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
}

void AVRPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UVRHandAnimInstance>(GetMesh()->GetAnimInstance());
}

void AVRPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	//InitVRSetting();
	SpawnVRHands();
}

void AVRPlayer::OnDeath()
{
	SetActorRotation(FRotator(0.0f, -90.0f, 0.0f));
	bUseControllerRotationYaw = false;
	DeathSequence->GetSequencePlayer()->Play();
}

void AVRPlayer::InitVRSetting()
{
	// HMD가 연결되어 있으면 발 기준 트래킹 원점으로 설정합니다.
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::View);
		LOG(TEXT("HMD 감지 — 트래킹 원점을 LocalFloor 설정했습니다."));
	}
	else
	{
		LOG(TEXT("HMD가 감지되지 않았습니다. 일반 카메라 모드로 실행됩니다."));
	}
}

void AVRPlayer::SpawnVRHands()
{
	if (!LeftHandClass || !RightHandClass)
	{
		LOG(TEXT("손 클래스가 설정되지 않았습니다. 블루프린트에서 LeftHandClass와 RightHandClass를 할당해 주세요."));
		return;
	}
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	
	LeftHand = GetWorld()->SpawnActor<AVRHand>(LeftHandClass, FTransform::Identity, SpawnParams);
	if (LeftHand)
	{
		LeftHand->AttachToComponent(VROrigin, FAttachmentTransformRules::SnapToTargetIncludingScale);
		LeftHand->SetupPlayerInputComponent(EIC);
	}
	
	RightHand = GetWorld()->SpawnActor<AVRHand>(RightHandClass, FTransform::Identity, SpawnParams);
	if (RightHand)
	{
		RightHand->AttachToComponent(VROrigin, FAttachmentTransformRules::SnapToTargetIncludingScale);
		RightHand->SetupPlayerInputComponent(EIC);
	}
}


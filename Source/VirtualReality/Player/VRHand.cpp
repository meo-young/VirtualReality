#include "VRHand.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "MotionControllerComponent.h"
#include "VirtualReality.h"
#include "Animation/VRHandAnimInstance.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetInteractionComponent.h"


AVRHand::AVRHand()
{
	// MotionController 초기화
	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	SetRootComponent(MotionController);
	
	// HandMesh 초기화
	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh"));
	HandMesh->SetupAttachment(MotionController);
	
	// WidgetInteractionComponent 초기화
	WidgetInteractionComponent = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteractionComponent"));
	WidgetInteractionComponent->SetupAttachment(HandMesh);
	
	// GrabCollision 초기화
	GrabCollision = CreateDefaultSubobject<USphereComponent>(TEXT("GrabCollision"));
	GrabCollision->SetupAttachment(HandMesh);
}

void AVRHand::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (HandGraspAction)
		{
			EIC->BindAction(HandGraspAction, ETriggerEvent::Triggered, this, &AVRHand::DoHandGrasp);
			EIC->BindAction(HandGraspAction, ETriggerEvent::Canceled, this, &AVRHand::StopHandGrasp);
			EIC->BindAction(HandGraspAction, ETriggerEvent::Completed, this, &AVRHand::StopHandGrasp);
		}
		
		if (HandIndexCurlAction)
		{
			EIC->BindAction(HandIndexCurlAction, ETriggerEvent::Triggered, this, &AVRHand::DoHandIndexCurl);
			EIC->BindAction(HandIndexCurlAction, ETriggerEvent::Canceled, this, &AVRHand::StopHandIndexCurl);
			EIC->BindAction(HandIndexCurlAction, ETriggerEvent::Completed, this, &AVRHand::StopHandIndexCurl);
		}
		
		if (HandPointAction)
		{
			EIC->BindAction(HandPointAction, ETriggerEvent::Started, this, &AVRHand::DoHandPoint);
			EIC->BindAction(HandPointAction, ETriggerEvent::Canceled, this, &AVRHand::DoHandPoint);
			EIC->BindAction(HandPointAction, ETriggerEvent::Completed, this, &AVRHand::StopHandPoint);
		}
		
		if (HandThumbUpAction)
		{
			EIC->BindAction(HandThumbUpAction, ETriggerEvent::Started, this, &AVRHand::DoHandThumbUp);
			EIC->BindAction(HandThumbUpAction, ETriggerEvent::Canceled, this, &AVRHand::DoHandThumbUp);
			EIC->BindAction(HandThumbUpAction, ETriggerEvent::Completed, this, &AVRHand::StopHandThumbUp);
		}
	}
}

void AVRHand::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	switch (HandType)
	{
		case EControllerHand::Left:
			MotionController->MotionSource = FName("Left");
			break;
		case EControllerHand::Right:	
			MotionController->MotionSource = FName("Right");
			break;
		default:
			break;
	}
}

void AVRHand::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UVRHandAnimInstance>(HandMesh->GetAnimInstance());
}

void AVRHand::BeginPlay()
{
	Super::BeginPlay();
	
	AnimInstance->bIsMirror = bMirrorAnimation;
}

void AVRHand::DoHandGrasp(const FInputActionValue& InValue)
{
	const float ActionValue = InValue.Get<float>();
	
	LOG(TEXT("발동"))
	AnimInstance->PoseAlphaGrasp = ActionValue;
}

void AVRHand::DoHandIndexCurl(const FInputActionValue& InValue)
{
	const float ActionValue = InValue.Get<float>();
	LOG(TEXT("발동"))

	AnimInstance->PoseAlphaIndexCurl = ActionValue;
}

void AVRHand::DoHandPoint()
{	LOG(TEXT("발동"))

	AnimInstance->PoseAlphaPoint = 0.0f;
}

void AVRHand::DoHandThumbUp()
{	LOG(TEXT("발동"))

	AnimInstance->PoseAlphaThumbUp = 0.0f;
}

void AVRHand::StopHandGrasp()
{
	AnimInstance->PoseAlphaGrasp = 0.0f;
}

void AVRHand::StopHandIndexCurl()
{
	AnimInstance->PoseAlphaIndexCurl = 0.0f;
}

void AVRHand::StopHandPoint()
{
	AnimInstance->PoseAlphaPoint = 1.0f;
}

void AVRHand::StopHandThumbUp()
{
	AnimInstance->PoseAlphaThumbUp = 1.0f;
}

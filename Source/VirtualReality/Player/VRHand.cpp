#include "VRHand.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "MotionControllerComponent.h"
#include "VirtualReality.h"
#include "Animation/VRHandAnimInstance.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Define/Define.h"
#include "Interface/Interactable.h"


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
	GrabCollision->SetSphereRadius(15.0f);
	GrabCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GrabCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	GrabCollision->SetCollisionResponseToChannel(ECC_GRABBABLE, ECR_Overlap);
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
		
		if (GrabAction)
		{
			EIC->BindAction(GrabAction, ETriggerEvent::Started, this, &AVRHand::GrabObject);
			EIC->BindAction(GrabAction, ETriggerEvent::Canceled, this, &AVRHand::ReleaseObject);
			EIC->BindAction(GrabAction, ETriggerEvent::Completed, this, &AVRHand::ReleaseObject);
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

void AVRHand::GrabObject()
{
	TArray<AActor*> OverlappedActors;
	GrabCollision->GetOverlappingActors(OverlappedActors);
	if (OverlappedActors.IsEmpty()) return;
	
	AActor* FirstActorUnderCollision = OverlappedActors[0];
	if (!FirstActorUnderCollision) return;
	
	CurrentlyGrabbedActor = TScriptInterface<IInteractable>(FirstActorUnderCollision);
	if (CurrentlyGrabbedActor)
	{
		CurrentlyGrabbedActor->OnGrab(HandMesh, GrabCollision->GetComponentLocation());
	}
}

void AVRHand::ReleaseObject()
{
	if (CurrentlyGrabbedActor)
	{
		CurrentlyGrabbedActor->OnRelease(HandMesh);
		CurrentlyGrabbedActor = nullptr;
	}
}


void AVRHand::DoHandGrasp(const FInputActionValue& InValue)
{
	const float ActionValue = InValue.Get<float>();
	
	AnimInstance->PoseAlphaGrasp = ActionValue;
}

void AVRHand::DoHandIndexCurl(const FInputActionValue& InValue)
{
	const float ActionValue = InValue.Get<float>();

	AnimInstance->PoseAlphaIndexCurl = ActionValue;
}

void AVRHand::DoHandPoint()
{	
	AnimInstance->PoseAlphaPoint = 0.0f;
}

void AVRHand::DoHandThumbUp()
{	
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

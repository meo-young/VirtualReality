#include "VRHand.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "MotionControllerComponent.h"
#include "VirtualReality.h"
#include "Animation/VRHandAnimInstance.h"
#include "Component/VRHapticComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Define/Define.h"
#include "Interface/Grabbable.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"


AVRHand::AVRHand()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// SceneComponent 초기화
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	// MotionController 초기화
	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	MotionController->SetupAttachment(Root);
	
	// 가상 HandMesh 초기화
	VirtualHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VirtualHandMesh"));
	VirtualHandMesh->SetupAttachment(MotionController);
	VirtualHandMesh->SetGenerateOverlapEvents(false);
	VirtualHandMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	VirtualHandMesh->SetClothMaxDistanceScale(ECR_Overlap);
	
	// PhysicsConstraint 초기화
	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
	PhysicsConstraint->SetupAttachment(MotionController);
	
	// HandMesh 초기화
	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh"));
	HandMesh->SetupAttachment(Root);
	HandMesh->SetSimulatePhysics(true);
	HandMesh->SetCollisionProfileName(FName("PhysicsActor"));
	
	// WidgetInteractionComponent 초기화
	WidgetInteractionComponent = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteractionComponent"));
	WidgetInteractionComponent->SetupAttachment(HandMesh);
	
	// GrabCollision 초기화
	GrabCollision = CreateDefaultSubobject<USphereComponent>(TEXT("GrabCollision"));
	GrabCollision->SetupAttachment(HandMesh);
	GrabCollision->SetSphereRadius(10.0f);
	GrabCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GrabCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	GrabCollision->SetCollisionResponseToChannel(ECC_GRABBABLE, ECR_Overlap);

	// HapticComponent 초기화
	HapticComponent = CreateDefaultSubobject<UVRHapticComponent>(TEXT("HapticComponent"));
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
			BoneName = FName("hand_l");
			break;
		case EControllerHand::Right:	
			MotionController->MotionSource = FName("Right");
			BoneName = FName("hand_r");
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

	UpdateHandPhysicsBelow(BoneName, true, 0.15f);

	// HapticComponent에 손 타입을 전달합니다.
	HapticComponent->Initialize(HandType);
}

void AVRHand::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	FVector CurrentLocation = MotionController->GetComponentLocation();
	CurrentCalculatedVelocity = (CurrentLocation - LastLocation) / DeltaSeconds;
	LastLocation = CurrentLocation;
}

void AVRHand::GrabObject()
{
	// GrabCollision과 겹쳐있는 액터들 중 첫 번째 액터를 가져옵니다.
	TArray<AActor*> OverlappedActors;
	GrabCollision->GetOverlappingActors(OverlappedActors);
	if (OverlappedActors.IsEmpty()) return;
	
	AActor* FirstActorUnderCollision = OverlappedActors[0];
	if (!FirstActorUnderCollision) return;
	
	// 해당 액터가 잡을 수 있는 액터라면 OnGrab을 호출합니다.
	// 만약, 이미 잡혀있는 상태라면 반환합니다.
	CurrentlyGrabbedActor = TScriptInterface<IGrabbable>(FirstActorUnderCollision);
	if (CurrentlyGrabbedActor)
	{
		if (CurrentlyGrabbedActor->IsHeld())
		{
			CurrentlyGrabbedActor = nullptr;
			return;
		}
		
		bIsGrabbing = true;
		CurrentGrabbableType = CurrentlyGrabbedActor->GetGrabbableType();
		CurrentlyGrabbedActor->OnGrab(HandMesh);
	}
}

void AVRHand::ReleaseObject()
{
	bIsGrabbing = false;
	
	if (CurrentlyGrabbedActor)
	{
		CurrentlyGrabbedActor->OnRelease(HandMesh);
		CurrentlyGrabbedActor = nullptr;
		UpdateHandPhysicsBelow(BoneName, true, 0.15f);
	}
}

void AVRHand::UpdateHandPhysicsBelow(FName InBoneName, bool bNewSimulate, float InBlendWeight)
{
	if (HandMesh)
	{
		HandMesh->SetAllBodiesBelowSimulatePhysics(InBoneName, bNewSimulate, true);
		HandMesh->SetAllBodiesBelowPhysicsBlendWeight(InBoneName, InBlendWeight, false, true);
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

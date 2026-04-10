#include "VRGrabbableActor.h"
#include "Components/BoxComponent.h"
#include "Player/VRHand.h"
#include "Component/VRHapticComponent.h"

AVRGrabbableActor::AVRGrabbableActor()
{
	GrabRegion = CreateDefaultSubobject<UBoxComponent>(TEXT("GrabBoxRegion"));
	GrabRegion->SetupAttachment(Mesh);
	GrabRegion->SetCollisionProfileName(TEXT("Grabbable"));
}

void AVRGrabbableActor::BeginPlay()
{
	Super::BeginPlay();
	
	bIsLocked = false;
}

void AVRGrabbableActor::OnGrab(USkeletalMeshComponent* InComponent)
{
	if (bIsLocked || bIsHeld) return;
	
	bIsHeld = true;
	
	CachedHand = Cast<AVRHand>(InComponent->GetOwner());
	
	if (CachedHand)
	{
		// 레버를 당기는 동안 연속 진동을 재생합니다.
		CachedHand->GetHapticComponent()->PlayHaptic(GrabHapticScale);
	}
	
	DoGrab(InComponent);
}

void AVRGrabbableActor::OnRelease(USkeletalMeshComponent* InComponent)
{
	if (CachedHand)
	{
		CachedHand->GetHapticComponent()->StopHaptic();
	}
	
	DoRelease(InComponent);

	CachedHand = nullptr;
	bIsHeld = false;
}

void AVRGrabbableActor::DoGrab(USkeletalMeshComponent* InComponent)
{
}

void AVRGrabbableActor::DoRelease(USkeletalMeshComponent* InComponent)
{
}


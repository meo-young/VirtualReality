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

void AVRGrabbableActor::OnGrab(USkeletalMeshComponent* InComponent, const FVector& GrabLocation)
{
	CachedHand = Cast<AVRHand>(InComponent->GetOwner());
	if (CachedHand)
	{
		CachedHand->GetHapticComponent()->PlayHaptic(GrabHapticFrequency, GrabHapticAmplitude);
	}
}

void AVRGrabbableActor::OnRelease(USkeletalMeshComponent* InComponent)
{
	if (CachedHand)
	{
		CachedHand->GetHapticComponent()->StopHaptic();
	}

	CachedHand = nullptr;
}


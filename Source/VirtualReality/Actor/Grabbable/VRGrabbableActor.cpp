#include "VRGrabbableActor.h"
#include "Components/BoxComponent.h"

AVRGrabbableActor::AVRGrabbableActor()
{
	GrabRegion = CreateDefaultSubobject<UBoxComponent>(TEXT("GrabBoxRegion"));
	GrabRegion->SetupAttachment(Mesh);
	GrabRegion->SetCollisionProfileName(TEXT("Grabbable"));
}

void AVRGrabbableActor::OnGrab(USkeletalMeshComponent* InComponent, const FVector& GrabLocation)
{
}

void AVRGrabbableActor::OnRelease(USkeletalMeshComponent* InComponent)
{
}


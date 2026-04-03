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
	if (!InComponent) return;
	
	switch (GrabType)
	{
		case EGrabType::Free:
			Mesh->SetSimulatePhysics(false);
			bIsHeld = Mesh->AttachToComponent(InComponent, FAttachmentTransformRules::KeepWorldTransform);
			if (bIsHeld)
			{
				GrabbedBySkeletalMesh = InComponent;
			}
			break;
		case EGrabType::Snap:
			break;
		case EGrabType::None:
			break;
	}
}

void AVRGrabbableActor::OnRelease(USkeletalMeshComponent* InComponent)
{
	if (!InComponent) return;
	
	switch (GrabType)
	{
	case EGrabType::Free:
		if (bIsHeld)
		{
			if (GrabbedBySkeletalMesh == InComponent)
			{
				Mesh->SetSimulatePhysics(true);
				//DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				//GrabbedBySkeletalMesh = nullptr;
				bIsHeld = false;
			}
		}
		break;
	case EGrabType::Snap:
		break;
	case EGrabType::None:
		break;
	}
}

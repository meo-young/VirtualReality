#include "GrabbableFree.h"

void AGrabbableFree::OnGrab(USkeletalMeshComponent* InComponent, const FVector& GrabLocation)
{
	Super::OnGrab(InComponent, GrabLocation);
	
	Mesh->SetSimulatePhysics(false);
	bIsHeld = Mesh->AttachToComponent(InComponent, FAttachmentTransformRules::KeepWorldTransform);
	if (bIsHeld)
	{
		GrabbedBySkeletalMesh = InComponent;
	}
}

void AGrabbableFree::OnRelease(USkeletalMeshComponent* InComponent)
{
	Super::OnRelease(InComponent);
	
	if (bIsHeld)
	{
		if (GrabbedBySkeletalMesh == InComponent)
		{
			Mesh->SetSimulatePhysics(true);
			bIsHeld = false;
		}
	}
}

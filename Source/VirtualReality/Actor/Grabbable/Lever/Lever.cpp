#include "Lever.h"
#include "VirtualReality.h"
#include "Components/BoxComponent.h"

ALever::ALever()
{
	LeverMesh = CreateDefaultSubobject<USkeletalMeshComponent>("LeverMesh");
	LeverMesh->SetupAttachment(Mesh);
	
	GrabRegion->SetupAttachment(LeverMesh);
	GrabRegion->SetRelativeLocation(FVector(0.0f, -14.0f, 0.0f));
	GrabRegion->SetBoxExtent(FVector(16.0f, 16.0f, 8.0f));
	
	GrabbableType = EGrabbableType::Lever;
}

void ALever::OnGrab(USkeletalMeshComponent* InComponent, const FVector& GrabLocation)
{
	Super::OnGrab(InComponent, GrabLocation);
	
	// 손 컴포넌트를 레버의 GrabSocket에 부착합니다.
	InComponent->SetAllBodiesSimulatePhysics(false);
	InComponent->AttachToComponent(
		LeverMesh,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		GrabSocketName
	);
}

void ALever::OnRelease(USkeletalMeshComponent* InComponent)
{
	Super::OnRelease(InComponent);
	
	// 손 컴포넌트를 레버에서 분리합니다.
	InComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	InComponent->SetAllBodiesSimulatePhysics(true);
}

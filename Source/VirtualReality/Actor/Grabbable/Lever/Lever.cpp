#include "Lever.h"
#include "VirtualReality.h"
#include "Component/VRHapticComponent.h"
#include "Components/BoxComponent.h"
#include "Player/VRHand.h"

ALever::ALever()
{
	PrimaryActorTick.bCanEverTick = true;

	GrabRegion->SetupAttachment(LeverMesh);
	GrabRegion->SetRelativeLocation(FVector(0.0f, -14.0f, 0.0f));
	GrabRegion->SetBoxExtent(FVector(16.0f, 16.0f, 8.0f));

	GrabbableType = EGrabbableType::Lever;
}

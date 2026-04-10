#include "Lever.h"
#include "VirtualReality.h"
#include "Component/VRHapticComponent.h"
#include "Components/BoxComponent.h"
#include "Player/VRHand.h"

ALever::ALever()
{
	PrimaryActorTick.bCanEverTick = true;

	// Grab이 가능한 영역인 BoxCollision을 초기화합니다.
	GrabRegion->SetupAttachment(LeverMesh);
	GrabRegion->SetRelativeLocation(FVector(0.0f, -14.0f, 0.0f));
	GrabRegion->SetBoxExtent(FVector(16.0f, 16.0f, 8.0f));

	// Animation 처리를 위한 Grabbable 타입을 지정합니다.
	GrabbableType = EGrabbableType::Lever;
	
	// 레버의 작동 축을 결정합니다.
	LeverAxis = ELeverAxis::Z;
}

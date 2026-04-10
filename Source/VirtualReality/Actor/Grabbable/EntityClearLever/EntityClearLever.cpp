#include "EntityClearLever.h"
#include "VirtualReality.h"
#include "Components/BoxComponent.h"

AEntityClearLever::AEntityClearLever()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// 메시의 전체적인 스케일을 조정합니다.
	Root->SetRelativeScale3D(FVector(0.15f, 0.15f, 0.15f));

	// Grab에 사용할 Mesh를 초기화합니다.
	LeverMesh->SetRelativeLocation(FVector(0.0f, -10.0f, -24.0f));
	LeverMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, -30.0f));

	// Grab이 가능한 영역인 BoxCollision을 초기화합니다.
	GrabRegion->SetupAttachment(LeverMesh);
	GrabRegion->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	GrabRegion->SetRelativeRotation(FRotator(0.0f, 0.0f, 30.0f));
	GrabRegion->SetBoxExtent(FVector(32.0f, 32.0f, 32.0f));

	// Animation 처리를 위한 Grabbable 타입을 지정합니다.
	GrabbableType = EGrabbableType::EntityClearLever;
}
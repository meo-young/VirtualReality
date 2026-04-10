#pragma once

#include "CoreMinimal.h"
#include "Actor/Grabbable/VRGrabbableActor.h"
#include "GrabbableFree.generated.h"

UCLASS()
class VIRTUALREALITY_API AGrabbableFree : public AVRGrabbableActor
{
	GENERATED_BODY()
	
public:
	virtual void OnGrab(USkeletalMeshComponent* InComponent) override;
	virtual void OnRelease(USkeletalMeshComponent* InComponent) override;


};

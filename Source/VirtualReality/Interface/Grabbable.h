#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Grabbable.generated.h"

UINTERFACE()
class UGrabbable : public UInterface
{
	GENERATED_BODY()
};


class VIRTUALREALITY_API IGrabbable
{
	GENERATED_BODY()

public:
	virtual void OnGrab(USkeletalMeshComponent* InComponent, const FVector& GrabLocation) = 0;
	virtual void OnRelease(USkeletalMeshComponent* InComponent) = 0;

};

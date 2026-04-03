#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

UINTERFACE()
class UInteractable : public UInterface
{
	GENERATED_BODY()
};


class VIRTUALREALITY_API IInteractable
{
	GENERATED_BODY()

public:
	virtual void OnGrab(USkeletalMeshComponent* InComponent, const FVector& GrabLocation) = 0;
	virtual void OnRelease(USkeletalMeshComponent* InComponent) = 0;

};

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Grabbable.generated.h"

UENUM(BlueprintType)
enum class EGrabbableType : uint8
{
	Free,
	Lever,
	EntityClearLever,
	Flash
};

UINTERFACE()
class UGrabbable : public UInterface
{
	GENERATED_BODY()
};


class VIRTUALREALITY_API IGrabbable
{
	GENERATED_BODY()

public:
	virtual void OnGrab(USkeletalMeshComponent* InComponent) = 0;
	virtual void OnRelease(USkeletalMeshComponent* InComponent) = 0;
	virtual EGrabbableType GetGrabbableType() const = 0;
	virtual uint8 IsHeld() const = 0;

};

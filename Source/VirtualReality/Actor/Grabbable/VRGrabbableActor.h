#pragma once

#include "CoreMinimal.h"
#include "Actor/VRActorBase.h"
#include "Interface/Interactable.h"
#include "VRGrabbableActor.generated.h"

class UBoxComponent;

UENUM(BlueprintType)
enum class EGrabType : uint8
{
	Free,
	Snap,
	None
};

UCLASS()
class VIRTUALREALITY_API AVRGrabbableActor : public AVRActorBase, public IInteractable
{
	GENERATED_BODY()
	
public:
	AVRGrabbableActor();
	
public:
	virtual void OnGrab(USkeletalMeshComponent* InComponent, const FVector& GrabLocation) override;
	virtual void OnRelease(USkeletalMeshComponent* InComponent) override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<UBoxComponent> GrabRegion;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|타입")
	EGrabType GrabType;
	
protected:
	uint8 bIsHeld : 1;
	
	UPROPERTY()
	USkeletalMeshComponent* GrabbedBySkeletalMesh;
	
};

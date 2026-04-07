#pragma once

#include "CoreMinimal.h"
#include "Actor/VRActorBase.h"
#include "Interface/Grabbable.h"
#include "VRGrabbableActor.generated.h"

class UBoxComponent;

UCLASS()
class VIRTUALREALITY_API AVRGrabbableActor : public AVRActorBase, public IGrabbable
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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|타입")
	EGrabbableType GrabbableType = EGrabbableType::Free;
	
protected:
	UPROPERTY()
	USkeletalMeshComponent* GrabbedBySkeletalMesh;
	
	uint8 bIsHeld : 1;

public:
	FORCEINLINE virtual EGrabbableType GetGrabbableType() const override { return GrabbableType; }
	
};

#pragma once

#include "CoreMinimal.h"
#include "Actor/VRActorBase.h"
#include "Interface/Grabbable.h"
#include "VRGrabbableActor.generated.h"

class UBoxComponent;
class AVRHand;

UCLASS()
class VIRTUALREALITY_API AVRGrabbableActor : public AVRActorBase, public IGrabbable
{
	GENERATED_BODY()
	

// Lifecycle Section	
public:
	AVRGrabbableActor();
	virtual void BeginPlay() override;
	

// IGrabbable Interface	
public:
	virtual void OnGrab(USkeletalMeshComponent* InComponent) override;
	virtual void OnRelease(USkeletalMeshComponent* InComponent) override;
	
protected:
	virtual void DoGrab(USkeletalMeshComponent* InComponent);
	virtual void DoRelease(USkeletalMeshComponent* InComponent);
	

// Component Section	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<UBoxComponent> GrabRegion;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|타입")
	EGrabbableType GrabbableType = EGrabbableType::Free;
	
	
// Haptic Feedback Section	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float GrabHapticScale = 0.4f;
	
	
// Cached Section
	UPROPERTY()
	USkeletalMeshComponent* GrabbedBySkeletalMesh;

	UPROPERTY()
	TObjectPtr<AVRHand> CachedHand;
	
	
// State Section	
	uint8 bIsHeld : 1;
	uint8 bIsLocked : 1;

	
// Getter, Setter Section	
public:
	FORCEINLINE virtual EGrabbableType GetGrabbableType() const override final { return GrabbableType; }
	FORCEINLINE virtual uint8 IsHeld() const override final { return bIsHeld; }
	
};

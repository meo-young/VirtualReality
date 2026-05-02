#pragma once

#include "CoreMinimal.h"
#include "Actor/Grabbable/VRGrabbableActor.h"
#include "Interface/Interactable.h"
#include "Flash.generated.h"

class USpotLightComponent;

UCLASS()
class VIRTUALREALITY_API AFlash : public AVRGrabbableActor, public IInteractable
{
	GENERATED_BODY()
	
	
// Lifecycle Section	
public:
	AFlash();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	
// IGrabbable Interface	
public:
	virtual void DoGrab(USkeletalMeshComponent* InComponent) override;
	virtual void DoRelease(USkeletalMeshComponent* InComponent) override;
	
	
// IInteractable Interface	
public:	
	virtual void Interact() override;
	

// Member Function
private:	
	void SetEnableLightVisibility(bool InEnableLightVisibility);
	bool IsIrradiateEventZone();
	
	
// Component Section	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<USpotLightComponent> FlashLight;
	
	
// Grab Section	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|소켓")
	FName RightGrabSocketName = FName(TEXT("FlashSocket_R"));
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|소켓")
	FName LeftGrabSocketName = FName(TEXT("FlashSocket_L"));


};

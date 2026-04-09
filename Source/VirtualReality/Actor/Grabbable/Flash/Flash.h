#pragma once

#include "CoreMinimal.h"
#include "Actor/Grabbable/VRGrabbableActor.h"
#include "Flash.generated.h"

class USpotLightComponent;

UCLASS()
class VIRTUALREALITY_API AFlash : public AVRGrabbableActor
{
	GENERATED_BODY()
	
	
// Lifecycle Section	
public:
	AFlash();
	
	
// IGrabbable Interface	
public:
	virtual void OnGrab(USkeletalMeshComponent* InComponent, const FVector& GrabLocation) override;
	virtual void OnRelease(USkeletalMeshComponent* InComponent) override;
	
	
// Component Section	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<USpotLightComponent> FlashLight;
	
	
// Grab Section	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|소켓")
	FName GrabSocketName = FName(TEXT("GrabSocket"));


};

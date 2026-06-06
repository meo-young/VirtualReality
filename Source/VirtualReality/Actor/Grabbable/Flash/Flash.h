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
	
	
// IInteractable Interface
public:
	virtual void Interact() override;

	/** 후레쉬 라이트를 강제로 켜거나 끕니다. (외부에서 호출) */
	void SetLightEnabled(bool bEnabled);


// Member Function
private:
	/** 왼손을 찾아 후레쉬를 소켓에 영구 부착합니다. 손이 아직 없으면 잠시 후 재시도합니다. */
	void TryAttachToHand();

	void SetEnableLightVisibility(bool InEnableLightVisibility);
	bool IsIrradiateEventZone();
	
	
public:
	uint8 bIsIrradiateEventZone : 1 = false;
	
	
// Component Section	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<USpotLightComponent> FlashLight;
	
	
// Grab Section	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|소켓")
	FName RightGrabSocketName = FName(TEXT("FlashSocket_R"));
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|소켓")
	FName LeftGrabSocketName = FName(TEXT("FlashSocket_L"));


private:
	/** 왼손 부착 재시도 타이머 핸들입니다. */
	FTimerHandle AttachRetryTimerHandle;

};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VRPlayer.generated.h"

class UVRHandAnimInstance;
class AVRHand;
class UCameraComponent;

UCLASS()
class VIRTUALREALITY_API AVRPlayer : public ACharacter
{
	GENERATED_BODY()
	
public:
	AVRPlayer();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	
protected:
	/** VR 트래킹 원점을 초기화하는 함수입니다. */
	void InitVRSetting();
	
	/** VR 손 액터를 스폰하는 함수입니다. LeftHandClass와 RightHandClass가 설정되어 있어야 합니다. */
	void SpawnVRHands();
	
	
// Component Section	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<USceneComponent> VROrigin;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<UCameraComponent> CameraComponent;
	
	
// Hand Class Section	
protected:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|손")
	TSubclassOf<AVRHand> LeftHandClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|손")
	TSubclassOf<AVRHand> RightHandClass;
	
	
// Cached Variable Section	
private:
	UPROPERTY()
	TObjectPtr<AVRHand> LeftHand;
	
	UPROPERTY()
	TObjectPtr<AVRHand> RightHand;
	
	UPROPERTY()
	TObjectPtr<UVRHandAnimInstance> AnimInstance;
	
	UPROPERTY()
	TObjectPtr<UEnhancedInputComponent> EIC;

};

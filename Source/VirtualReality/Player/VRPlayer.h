#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VRPlayer.generated.h"

class ALevelSequenceActor;
class UInputMappingContext;
class UVRHandAnimInstance;
class AVRHand;
class UCameraComponent;

DECLARE_MULTICAST_DELEGATE(FOnPlayerDeathDelegate);

UCLASS()
class VIRTUALREALITY_API AVRPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AVRPlayer();
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void OnDeath();


// Delegate Section
public:
	FOnPlayerDeathDelegate OnPlayerDeathDelegate;
	
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
	

// Sequence Section	
public:
	UPROPERTY(EditAnywhere, Category = "변수|시퀀스")
	TObjectPtr<ALevelSequenceActor> DeathSequence;
	
	
// Hand Class Section	
protected:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|손")
	TSubclassOf<AVRHand> LeftHandClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|손")
	TSubclassOf<AVRHand> RightHandClass;
	
	
// Input Section
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|입력")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|입력")
	TObjectPtr<UInputMappingContext> HandsMappingContext;
	
	
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

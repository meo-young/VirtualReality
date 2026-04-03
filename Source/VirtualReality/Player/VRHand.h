#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VRHand.generated.h"

struct FInputActionValue;
class UVRHandAnimInstance;
class UInputAction;
class USphereComponent;
class UWidgetInteractionComponent;
class UMotionControllerComponent;

UCLASS()
class VIRTUALREALITY_API AVRHand : public AActor
{
	GENERATED_BODY()
	
public:
	AVRHand();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	
public:
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
	
protected:
	void DoHandGrasp(const FInputActionValue& InValue);
	void DoHandIndexCurl(const FInputActionValue& InValue);
	void DoHandPoint();
	void DoHandThumbUp();
	
	void StopHandGrasp();
	void StopHandIndexCurl();
	void StopHandPoint();
	void StopHandThumbUp();
	
// Component Section	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<UMotionControllerComponent> MotionController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<USkeletalMeshComponent> HandMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<UWidgetInteractionComponent> WidgetInteractionComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<USphereComponent> GrabCollision;
	
	
// Input Section
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|입력")
	TObjectPtr<UInputAction> HandGraspAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|입력")
	TObjectPtr<UInputAction> HandIndexCurlAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|입력")
	TObjectPtr<UInputAction> HandPointAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|입력")
	TObjectPtr<UInputAction> HandThumbUpAction;


// Variable Section	
protected:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|손")
	EControllerHand HandType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|손")
	uint8 bMirrorAnimation : 1 = false;
	
	
// Cached Section	
private:
	UPROPERTY()
	TObjectPtr<UVRHandAnimInstance> AnimInstance;
	
};

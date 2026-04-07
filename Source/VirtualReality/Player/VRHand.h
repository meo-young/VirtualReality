#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VRHand.generated.h"

enum class EGrabbableType : uint8;
class UPhysicsConstraintComponent;
class IGrabbable;
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
	virtual void Tick(float DeltaSeconds) override;
	
public:
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
	
	
// Action Binding Function Section	
protected:
	// HandGraspAction Binding Function
	void DoHandGrasp(const FInputActionValue& InValue);
	void StopHandGrasp();
	
	// HandIndexCurl Binding Function
	void DoHandIndexCurl(const FInputActionValue& InValue);
	void StopHandIndexCurl();
	
	// HandPoint Binding Function
	void DoHandPoint();
	void StopHandPoint();
	
	// HandThumbUp Binding Function
	void DoHandThumbUp();
	void StopHandThumbUp();
	
	// GrabAction Binding Function
	void GrabObject();
	void ReleaseObject();
	
private:
	/** 애니메이션과 물리 엔진의 비율을 설정하는 함수입니다. */
	void UpdateHandPhysicsBelow(FName InBoneName, bool bNewSimulate, float InBlendWeight);
	
	
// Component Section	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<UMotionControllerComponent> MotionController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<USkeletalMeshComponent> HandMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<USkeletalMeshComponent> VirtualHandMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<UPhysicsConstraintComponent> PhysicsConstraint;
	
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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|입력")
	TObjectPtr<UInputAction> GrabAction;


// Variable Section	
protected:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|손")
	EControllerHand HandType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|손")
	uint8 bMirrorAnimation : 1 = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|손")
	FName BoneName;
	
private:
	uint8 bIsGrabbing : 1 = false;
	EGrabbableType CurrentGrabbableType;
	
	
// Cached Section	
private:
	UPROPERTY()
	TObjectPtr<UVRHandAnimInstance> AnimInstance;
	
	TScriptInterface<IGrabbable> CurrentlyGrabbedActor;
	
	FVector LastLocation;
	FVector CurrentCalculatedVelocity;
	
public:
	FORCEINLINE FVector GetHandVelocity() const { return CurrentCalculatedVelocity; }
	FORCEINLINE uint8 GetIsGrabbing() const { return bIsGrabbing; }
	FORCEINLINE EGrabbableType GetCurrentGrabbableType() const { return CurrentGrabbableType; }
	
};
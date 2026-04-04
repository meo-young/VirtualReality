#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VRHand.generated.h"

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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|입력")
	TObjectPtr<UInputAction> GrabAction;


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
	
	TScriptInterface<IGrabbable> CurrentlyGrabbedActor;
	
	FVector LastLocation;
	FVector CurrentCalculatedVelocity;
	
public:
	FORCEINLINE FVector GetHandVelocity() const { return CurrentCalculatedVelocity; }
	
};
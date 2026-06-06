#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MotionControllerComponent.h"
#include "VRHand.generated.h"

class IInteractable;
enum class EGrabbableType : uint8;
class UPhysicsConstraintComponent;
class IGrabbable;
struct FInputActionValue;
class UVRHandAnimInstance;
class UInputAction;
class USphereComponent;
class UWidgetInteractionComponent;
class UVRHapticComponent;

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

	/** 모션 컨트롤러의 추적 업데이트를 중지하여 손을 현재 위치에 고정합니다. */
	void FreezeMotionController();
	
	
// Action Binding Function Section		
public:
	// GrabAction Binding Function
	void GrabObject();
	void ReleaseObject();

protected:
	// HandGraspAction Binding Function
	void DoHandGrasp(const FInputActionValue& InValue);
	void StopHandGrasp();
	
	// HandIndexCurl Binding Function
	void DoInteract();
	void DoHandIndexCurl(const FInputActionValue& InValue);
	void StopHandIndexCurl();
	
	// HandPoint Binding Function
	void DoHandPoint();
	void StopHandPoint();
	
	// HandThumbUp Binding Function
	void DoHandThumbUp();
	void StopHandThumbUp();
	
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<UVRHapticComponent> HapticComponent;
	
	
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
	
	UPROPERTY()
	TObjectPtr<AActor> CurrentGrabbedActor;
	
	TScriptInterface<IGrabbable> CachedGrabbable;
	TScriptInterface<IInteractable> CachedInteractable;

	/** 손에 영구 부착된 후레쉬입니다. 설정되면 검지 트리거가 이 후레쉬의 켜고/끄기만 처리합니다. */
	TScriptInterface<IInteractable> AttachedFlashlight;

	FVector LastLocation;
	FVector CurrentCalculatedVelocity;

public:
	FORCEINLINE FVector GetHandVelocity() const { return CurrentCalculatedVelocity; }
	FORCEINLINE uint8 GetIsGrabbing() const { return bIsGrabbing; }
	FORCEINLINE EGrabbableType GetCurrentGrabbableType() const { return CurrentGrabbableType; }
	FORCEINLINE FVector GetMotionControllerLocation() const { return MotionController->GetComponentLocation(); }
	FORCEINLINE UVRHapticComponent* GetHapticComponent() const { return HapticComponent; }
	FORCEINLINE EControllerHand GetHandType() const { return HandType; }
	FORCEINLINE USkeletalMeshComponent* GetHandMesh() const { return HandMesh; }

	/** 손에 영구 부착할 후레쉬를 등록하고, 해당 GrabbableType에 맞는 손 애니메이션을 적용합니다. */
	FORCEINLINE void SetAttachedFlashlight(const TScriptInterface<IInteractable>& InFlashlight, EGrabbableType InGrabbableType)
	{
		AttachedFlashlight = InFlashlight;

		// 그랩 과정을 거치지 않으므로, 애니메이션이 참조하는 상태를 직접 맞춰줍니다.
		CurrentGrabbableType = InGrabbableType;
		bIsGrabbing = true;
	}
	
};
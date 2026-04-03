#pragma once

#include "CoreMinimal.h"
#include "Abstract/CharacterBase.h"
#include "PlayerCharacter.generated.h"

class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class UMotionControllerComponent;
struct FInputActionValue;

UCLASS()
class VIRTUALREALITY_API APlayerCharacter : public APawn
{
	GENERATED_BODY()
	
public:
	APlayerCharacter();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void NotifyControllerChanged() override;
	
private:
	/** HMD 트래킹 원점을 초기화하는 함수입니다. */
	void InitVRSetting();
	
	
// Component Section
protected:
	/** VR 트래킹 원점 컴포넌트입니다. (발 기준) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<USceneComponent> VROrigin;

	/** HMD에 연결되는 카메라 컴포넌트입니다. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<UCameraComponent> VRCamera;

	/** 왼손 모션 컨트롤러 컴포넌트입니다. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<UMotionControllerComponent> LeftController;
	
	/** 왼손 메쉬 컴포넌트입니다. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<USkeletalMeshComponent> LeftHandMesh;
	
	/** 오른손 모션 컨트롤러 컴포넌트입니다. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<UMotionControllerComponent> RightController;
	
	/** 오른손 메쉬 컴포넌트입니다. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<USkeletalMeshComponent> RightHandMesh;
	
	
	
// Input Section	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext;
	

// 	Variable Section
protected:	

	
};

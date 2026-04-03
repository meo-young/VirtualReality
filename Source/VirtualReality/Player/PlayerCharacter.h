#pragma once

#include "CoreMinimal.h"
#include "Abstract/CharacterBase.h"
#include "PlayerCharacter.generated.h"

class UInputAction;
class UCameraComponent;
class UMotionControllerComponent;
struct FInputActionValue;

UCLASS()
class VIRTUALREALITY_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	APlayerCharacter();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
private:
	/** HMD 트래킹 원점을 초기화하는 함수입니다. */
	void InitVRSetting();

	/** HMD 기기의 위치에 맞게 캐릭터를 이동시킵니다. */
	void LerpOffsetToHMD();
	
	/** 플레이어의 눈높이에 따라 CapsuleSize를 동적으로 변경합니다. */
	void ScaleCapsuleToHMD();
	
	/** 카메라 회전 값을 부드럽게 보간합니다. */
	void LerpControlRotation(const float InDeltaTime);
	
private:
	void DoMove(const FInputActionValue& InValue);
	void DoTurn(const FInputActionValue& InValue);
	
protected:
	/** VR 트래킹 원점 컴포넌트입니다. (발 기준) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	TObjectPtr<USceneComponent> VROrigin;

	/** HMD에 연결되는 카메라 컴포넌트입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	TObjectPtr<UCameraComponent> VRCamera;

	/** 왼손 모션 컨트롤러 컴포넌트입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	TObjectPtr<UMotionControllerComponent> LeftController;

	/** 오른손 모션 컨트롤러 컴포넌트입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	TObjectPtr<UMotionControllerComponent> RightController;
	
	/** 이동 입력 액션입니다. (Vector2D — 왼손 조이스틱) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> IA_Move;

	/** 회전 입력 액션입니다. (Axis1D — 오른손 조이스틱 X축) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> IA_Turn;
	
	/** 초당 회전 속도(도)입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float TurnSpeed = 90.f;

	/** 회전 보간 속도입니다. 값이 클수록 즉각적으로 회전합니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float TurnLagSpeed = 5.f;

private:
	/** 목표 Yaw 값입니다. (누적, 정규화 없음) */
	float TargetYaw;

	/** 현재 보간 중인 Yaw 값입니다. (누적, 정규화 없음) */
	float SmoothedYaw;
	
};

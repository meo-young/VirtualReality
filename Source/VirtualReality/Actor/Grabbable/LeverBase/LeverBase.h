#pragma once

#include "CoreMinimal.h"
#include "Actor/Grabbable/VRGrabbableActor.h"
#include "LeverBase.generated.h"

/** 레버를 당기는 기준 축을 나타내는 열거형입니다. */
UENUM(BlueprintType)
enum class ELeverAxis : uint8
{
	X UMETA(DisplayName = "X"),
	Y UMETA(DisplayName = "Y"),
	Z UMETA(DisplayName = "Z"),
};

DECLARE_MULTICAST_DELEGATE(FOnLeverReachedEndDelegate);

UCLASS()
class VIRTUALREALITY_API ALeverBase : public AVRGrabbableActor
{
	GENERATED_BODY()
	
	
// Lifecycle Section	
public:
	ALeverBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	
// IGrabbable Interface
public:
	virtual void DoGrab(USkeletalMeshComponent* InComponent) override;
	virtual void DoRelease(USkeletalMeshComponent* InComponent) override;	
	

// Delegate Section	
public:
	FOnLeverReachedEndDelegate OnLeverReachedEndDelegate;
	
	
// Member Function
protected:
	void UpdateLeverAngle(float DeltaTime);
	
private:
	float GetControllerAxisValue(const FVector& Location) const;
	FRotator GetRotationForAngle(float Angle) const;
	
	
// Component Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<USkeletalMeshComponent> LeverMesh;
	
	
// Grab Variable Section
protected:
	/** 레버의 시작 각도(Roll)입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float StartAngle = -30.f;

	/** 레버의 끝 각도(Roll)입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float EndAngle = 60.f;

	/** 최대 각도에 도달하기 위해 컨트롤러를 이동해야 하는 거리(cm)입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float MappingRange = 20.f;

	/** 레버를 잡고 있을 때의 보간 속도입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float ControlInterpSpeed = 8.f;

	/** EndAngle 도달 후 잠금 유지 시간(초)입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float LockDuration = 5.f;

	/** 잠금 해제 후 원위치로 복귀하는 보간 속도입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float ReturnInterpSpeed = 10.f;
	
	/** 레버를 당기는 기준 축입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	ELeverAxis LeverAxis = ELeverAxis::Y;

	/** 손을 부착할 소켓 이름입니다. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|소켓")
	FName GrabSocketName = FName(TEXT("GrabSocket"));
	
	
protected:
	float GrabStartControllerAxis = 0.f;
	float GrabStartAngle = 0.f;
	float CurrentAngle = 0.f;
	float LockTimer = 0.f;

	uint8 bReachedEndAngle : 1 = false;
	uint8 bIsReturning : 1 = false;

};

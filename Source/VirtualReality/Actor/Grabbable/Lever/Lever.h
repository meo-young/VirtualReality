#pragma once

#include "CoreMinimal.h"
#include "Actor/Grabbable/VRGrabbableActor.h"
#include "Lever.generated.h"

class AVRHand;

UCLASS()
class VIRTUALREALITY_API ALever : public AVRGrabbableActor
{
	GENERATED_BODY()

public:
	ALever();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


// IGrabbable Interface	
public:
	virtual void OnGrab(USkeletalMeshComponent* InComponent, const FVector& GrabLocation) override;
	virtual void OnRelease(USkeletalMeshComponent* InComponent) override;

	
// Member Function	
private:
	void UpdateLeverAngle(float DeltaTime);

	
// Component Section	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<USkeletalMeshComponent> LeverMesh;
	

	
// Grab Variable Section	
protected:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float MinAngle = -60.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float MaxAngle = 60.f;

	/** 최대 각도에 도달하기 위해 컨트롤러를 이동해야 하는 거리(cm)입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float MappingRange = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float InterpSpeed = 8.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float LockDuration = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float ReturnInterpSpeed = 10.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|소켓")
	FName GrabSocketName = FName(TEXT("GrabSocket"));

	
// Haptic Feedback Section	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float PullHapticFrequency = 0.4f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float PullHapticMaxAmplitude = 0.4f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float MinAngleHapticFrequency = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float MinAngleHapticAmplitude = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float MinAngleHapticDuration = 0.3f;

	
	
// Cached Section	
private:
	UPROPERTY()
	TObjectPtr<AVRHand> CachedHand;

	float GrabStartControllerZ = 0.f;
	float GrabStartAngle = 0.f;
	float CurrentAngle = 0.f;
	float LockTimer = 0.f;
	
	
// Flag Section	
	uint8 bReachedMinAngle : 1 = false;
	uint8 bIsLocked : 1 = false;
	uint8 bIsReturning : 1 = false;


public:
	FORCEINLINE float GetCurrentAngle() const { return CurrentAngle; }

};
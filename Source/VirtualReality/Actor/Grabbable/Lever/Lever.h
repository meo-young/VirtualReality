#pragma once

#include "CoreMinimal.h"
#include "Actor/Grabbable/VRGrabbableActor.h"
#include "Lever.generated.h"

UCLASS()
class VIRTUALREALITY_API ALever : public AVRGrabbableActor
{
	GENERATED_BODY()

	
// Lifecycle Section	
public:
	ALever();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


// IGrabbable Interface	
public:
	virtual void OnGrab(USkeletalMeshComponent* InComponent) override;
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
	float ControlInterpSpeed = 8.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float LockDuration = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float ReturnInterpSpeed = 10.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|소켓")
	FName GrabSocketName = FName(TEXT("GrabSocket"));
	
	
// Cached Section
private:
	float GrabStartControllerZ = 0.f;
	float GrabStartAngle = 0.f;
	float CurrentAngle = 0.f;
	float LockTimer = 0.f;
	
	
// Flag Section	
	uint8 bReachedMinAngle : 1 = false;
	uint8 bIsLocked : 1 = false;
	uint8 bIsReturning : 1 = false;


// Getter, Setter Section	
public:
	FORCEINLINE float GetCurrentAngle() const { return CurrentAngle; }

};
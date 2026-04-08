#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GenericPlatform/GenericPlatformInputDeviceMapper.h"
#include "VRHapticComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VIRTUALREALITY_API UVRHapticComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UVRHapticComponent();

public:
	/** 어느 손에 햅틱을 재생할지 초기화합니다. VRHand의 BeginPlay에서 호출합니다. */
	void Initialize(EControllerHand InHandType);

	/** 연속 햅틱을 재생합니다. (매 Tick 호출 가능) */
	void PlayHaptic(float Frequency, float Amplitude);

	/** 지정한 시간 동안 햅틱을 재생한 뒤 자동으로 중지합니다. */
	void PlayHapticBurst(float Frequency, float Amplitude, float Duration);

	/** 햅틱을 즉시 중지합니다. */
	void StopHaptic();

private:
	EControllerHand HandType = EControllerHand::Right;

	FTimerHandle BurstTimerHandle;

};

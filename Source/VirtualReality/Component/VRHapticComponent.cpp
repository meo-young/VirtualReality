#include "VRHapticComponent.h"
#include "VirtualReality.h"
#include "GameFramework/PlayerController.h"

UVRHapticComponent::UVRHapticComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UVRHapticComponent::Initialize(EControllerHand InHandType)
{
	HandType = InHandType;
}

void UVRHapticComponent::PlayHaptic(float Frequency, float Amplitude)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	PC->SetHapticsByValue(Frequency, Amplitude, HandType);
}

void UVRHapticComponent::PlayHapticBurst(float Frequency, float Amplitude, float Duration)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	// 이전 버스트 타이머가 남아있다면 취소합니다.
	GetWorld()->GetTimerManager().ClearTimer(BurstTimerHandle);

	PC->SetHapticsByValue(Frequency, Amplitude, HandType);

	// Duration 이후 자동으로 햅틱을 중지합니다.
	GetWorld()->GetTimerManager().SetTimer(
		BurstTimerHandle, this, &UVRHapticComponent::StopHaptic, Duration, false);
}

void UVRHapticComponent::StopHaptic()
{
	GetWorld()->GetTimerManager().ClearTimer(BurstTimerHandle);

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	PC->SetHapticsByValue(0.f, 0.f, HandType);
}

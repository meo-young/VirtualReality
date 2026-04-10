#include "VRHapticComponent.h"
#include "VirtualReality.h"
#include "GameFramework/PlayerController.h"

UVRHapticComponent::UVRHapticComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UVRHapticComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CachedPC = GetWorld()->GetFirstPlayerController();
}

void UVRHapticComponent::Initialize(EControllerHand InHandType)
{
	HandType = InHandType;
}

void UVRHapticComponent::PlayHaptic(const float InScale)
{
	CachedPC->PlayHapticEffect(HapticEffect, HandType, InScale, true);
}

void UVRHapticComponent::PlayHapticBurst(const float InScale, const float Duration)
{
	bIsBursting = true;
	
	// 이전 버스트 타이머가 남아있다면 취소합니다.
	GetWorld()->GetTimerManager().ClearTimer(BurstTimerHandle);

	CachedPC->PlayHapticEffect(HapticEffect, HandType, InScale, true);

	// Duration 이후 자동으로 햅틱을 중지합니다.
	GetWorld()->GetTimerManager().SetTimer(
		BurstTimerHandle, this, &UVRHapticComponent::StopBurstHaptic, Duration, false);
}

void UVRHapticComponent::StopHaptic()
{
	if (bIsBursting) return;

	CachedPC->StopHapticEffect(HandType);
}

void UVRHapticComponent::StopBurstHaptic()
{
	bIsBursting = false;
	
	GetWorld()->GetTimerManager().ClearTimer(BurstTimerHandle);

	CachedPC->StopHapticEffect(HandType);
}

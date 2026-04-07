#include "VRHandAnimInstance.h"
#include "VirtualReality.h"
#include "Player/VRHand.h"

void UVRHandAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	CachedHand = Cast<AVRHand>(GetOwningActor());
}

void UVRHandAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	CurrentPoseAlphaThumbUp = FMath::FInterpTo(
		CurrentPoseAlphaThumbUp, 
		PoseAlphaThumbUp, 
		DeltaSeconds, 
		InterpSpeed
	);

	CurrentPoseAlphaPoint = FMath::FInterpTo(
		CurrentPoseAlphaPoint, 
		PoseAlphaPoint, 
		DeltaSeconds, 
		InterpSpeed
	);
	
	if (!CachedHand) return;
	
	bIsGrabbing = CachedHand->GetIsGrabbing();
	CurrentGrabbableType = CachedHand->GetCurrentGrabbableType();
}

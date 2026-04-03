#include "VRHandAnimInstance.h"

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
}

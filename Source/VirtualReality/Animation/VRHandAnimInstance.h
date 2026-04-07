#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "VRHandAnimInstance.generated.h"

enum class EGrabbableType : uint8;
class AVRHand;

UCLASS()
class VIRTUALREALITY_API UVRHandAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수")
	float PoseAlphaGrasp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수")
	float PoseAlphaIndexCurl;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수")
	float PoseAlphaPoint;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수")
	float PoseAlphaThumbUp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수")
	float CurrentPoseAlphaThumbUp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수")
	float CurrentPoseAlphaPoint;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수")
	uint8 bIsMirror : 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수")
	uint8 bIsGrabbing : 1 = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수")
	EGrabbableType CurrentGrabbableType;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "변수")
	float InterpSpeed = 13.0f;
	
private:
	UPROPERTY()
	TObjectPtr<AVRHand> CachedHand;
	
};

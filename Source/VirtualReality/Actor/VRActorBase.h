#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VRActorBase.generated.h"

UCLASS()
class VIRTUALREALITY_API AVRActorBase : public AActor
{
	GENERATED_BODY()
	
	
// Lifecycle Section	
public:
	AVRActorBase();
	

// Component Section	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> Mesh;
	
	
// Haptic Feedback Section	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float BurstHapticFrequency = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float BurstHapticAmplitude = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float BurstHapticDuration = 0.3f;

};

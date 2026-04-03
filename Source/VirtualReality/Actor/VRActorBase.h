#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VRActorBase.generated.h"

UCLASS()
class VIRTUALREALITY_API AVRActorBase : public AActor
{
	GENERATED_BODY()
	
public:
	AVRActorBase();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> Mesh;

};

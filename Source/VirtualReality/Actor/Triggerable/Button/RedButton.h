#pragma once

#include "CoreMinimal.h"
#include "Actor/Triggerable/TriggerableActor.h"
#include "Interface/Sequenceable.h"
#include "RedButton.generated.h"

UCLASS()
class VIRTUALREALITY_API ARedButton : public ATriggerableActor, public ISequenceable
{
	GENERATED_BODY()
	
public:
	ARedButton();
	
public:
	virtual void OnSequenceStarted() override;
	virtual void OnSequenceEnded() override;
	
protected:
	virtual void OnTriggered() override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> ButtonMesh;
	
};

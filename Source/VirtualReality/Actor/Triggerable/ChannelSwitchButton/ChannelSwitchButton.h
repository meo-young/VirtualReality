#pragma once

#include "CoreMinimal.h"
#include "Actor/Triggerable/TriggerableActor.h"
#include "Interface/Sequenceable.h"
#include "ChannelSwitchButton.generated.h"

class AMonitor;
class UTextRenderComponent;

UCLASS()
class VIRTUALREALITY_API AChannelSwitchButton : public ATriggerableActor, public ISequenceable
{
	GENERATED_BODY()
	
// Lifecycle Function Section	
public:
	AChannelSwitchButton();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	

// Class Member Function	
protected:
	virtual void OnTriggered() override;
	void SwitchChannel();

	
//	ISequenceable Interface
protected:
	virtual void OnSequenceStarted() override;
	virtual void OnSequenceEnded() override;
	
	
// Component Section	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> ButtonMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<UTextRenderComponent> ChannelText;
	
	
// Variable Section
protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "변수|수치")
	uint8 ChannelNum;
	
private:
	FTimerHandle ChannelSwitchTimerHandle;
	
	
// Cached Section	
private:
	UPROPERTY()
	TObjectPtr<AMonitor> Monitor;

};

#pragma once

#include "CoreMinimal.h"
#include "Actor/Triggerable/TriggerableActor.h"
#include "Interface/Sequenceable.h"
#include "ChannelSwitchButton.generated.h"

class AMonitor;
class AClock;
class UTextRenderComponent;
class USpotLightComponent;

UCLASS()
class VIRTUALREALITY_API AChannelSwitchButton : public ATriggerableActor, public ISequenceable
{
	GENERATED_BODY()
	
// Lifecycle Function Section	
public:
	AChannelSwitchButton();
	virtual void BeginPlay() override;
	

// Member Function	
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
	TObjectPtr<UTextRenderComponent> TextRenderComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<USpotLightComponent> ButtonLight;


// Variable Section
private:
	FTimerHandle ChannelSwitchTimerHandle;

	/** 첫 트리거 여부를 표시합니다. true면 이미 Clock과 이벤트 사이클이 시작됨을 의미합니다. */
	uint8 bIsFirstTriggered : 1 = false;


// Cached Section
private:
	UPROPERTY()
	TObjectPtr<AMonitor> Monitor;

	UPROPERTY()
	TObjectPtr<AClock> Clock;

};

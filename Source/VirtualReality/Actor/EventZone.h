#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "EventZone.generated.h"

class ALevelSequenceActor;

UCLASS()
class VIRTUALREALITY_API AEventZone : public ATriggerBox
{
	GENERATED_BODY()

	
// LifeCycle Function	
public:
	AEventZone();
	virtual void BeginPlay() override;
	
	
// Member Function
public:
	void StartEventCycle();
	void StopEventCycle();
	void StopSequence();

	/** Flash 감지 후 지연 시간이 지나면 후레쉬를 끄고 시퀀스를 정지합니다. */
	void OnFlashStopElapsed();

	/** Flash가 이 구역을 감지했을 때 호출됩니다. */
	void OnFlashHit();

protected:
	void StartEvent();
	bool CanStartEvent() const;
	bool IsPlayerLookingAtZone() const;

	UFUNCTION()
	void OnSequenceEnded();

	
// Member Variable	
protected:
	UPROPERTY(EditInstanceOnly, Category = "변수|시퀀스")
	TObjectPtr<ALevelSequenceActor> EventSequence;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|시퀀스")
	float MinEventInterval = 30.0f;

	UPROPERTY(EditDefaultsOnly, Category = "변수|시퀀스")
	float MaxEventInterval = 45.0f;

	/** 플레이어 시선이 이 각도 이내에 들어오면 이벤트를 시작하지 않습니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|시퀀스")
	float PlayerLookAtAngleThreshold = 45.0f;

	/** 플레이어가 바라보는 상태에서 Flash 감지 시 Pause 후 후레쉬 OFF + Stop까지 대기 시간(초)입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|시퀀스")
	float FlashStopDelay = 2.0f;
	
	
// Timer
private:
	FTimerHandle EventTimerHandle;
	FTimerHandle FlashStopTimerHandle;
	

};

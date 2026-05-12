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

protected:
	void StartEvent();

	UFUNCTION()
	void OnSequenceEnded();

	
// Member Variable	
protected:
	UPROPERTY(EditInstanceOnly, Category = "변수|시퀀스")
	TObjectPtr<ALevelSequenceActor> EventSequence;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|시퀀스")
	float MinEventInterval = 60.0f;

	UPROPERTY(EditDefaultsOnly, Category = "변수|시퀀스")
	float MaxEventInterval = 90.0f;
	
	
// Timer	
private:
	FTimerHandle EventTimerHandle;
	

};

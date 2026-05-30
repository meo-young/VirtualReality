#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Clock.generated.h"

class ULevelSequencePlayer;
class ULevelSequence;
class USoundCue;

UCLASS()
class VIRTUALREALITY_API AClock : public AActor
{
	GENERATED_BODY()
	
public:	
	AClock();
	
public:
	void ActivateTimer();
	void PauseTimer();
	
private:
	void CountTime();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> ClockMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "변수|컴포넌트")
	TObjectPtr<UAudioComponent> AudioComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|사운드")
	TObjectPtr<USoundCue> ClockSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|수치")
	uint8 TimeInterval = 3;
	
	UPROPERTY(BlueprintReadOnly)
	FString CurrentTimeAsString;
	
private:
	FTimerHandle TimerHandle;
	int32 CurrentTime = 0;

};

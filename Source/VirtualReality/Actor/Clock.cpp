#include "Actor/Clock.h"
#include "LevelSequencePlayer.h"
#include "Components/AudioComponent.h"
#include "GameMode/MainGameMode.h"
#include "Sound/SoundCue.h"

AClock::AClock()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	ClockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Clock Mesh"));
	ClockMesh->SetupAttachment(Root);
	
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComponent->SetupAttachment(Root);
}

void AClock::ActivateTimer()
{
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ThisClass::CountTime, TimeInterval, true);
	AudioComponent->SetSound(ClockSound);
	AudioComponent->Play();
}

void AClock::PauseTimer()
{
	GetWorldTimerManager().ClearTimer(TimerHandle);
	AudioComponent->Stop();
}

void AClock::CountTime()
{
	CurrentTime += 1;
	
	const int32 Hour = CurrentTime / 60;
	const int32 Minute = CurrentTime % 60;
	
	if (CurrentTime / 60 >= 1)
	{
		PauseTimer();
	}
	
	CurrentTimeAsString = FString::Printf(TEXT("%02d:%02d"), Hour, Minute);
}

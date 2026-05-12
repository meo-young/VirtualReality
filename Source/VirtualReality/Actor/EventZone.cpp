#include "EventZone.h"

#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Components/ShapeComponent.h"
#include "Define/Define.h"
#include "Grabbable/Flash/Flash.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AEventZone::AEventZone()
{
	GetCollisionComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCollisionComponent()->SetCollisionResponseToChannel(ECC_FLASH, ECR_Block);
}

void AEventZone::BeginPlay()
{
	Super::BeginPlay();

	if (EventSequence)
	{
		if (ULevelSequencePlayer* Player = EventSequence->GetSequencePlayer())
		{
			Player->OnFinished.AddDynamic(this, &ThisClass::OnSequenceEnded);
		}
	}

}

void AEventZone::StartEventCycle()
{
	GetWorldTimerManager().SetTimer(EventTimerHandle, this, &ThisClass::StartEvent, FMath::FRandRange(MinEventInterval, MaxEventInterval), false);
}

void AEventZone::StopEventCycle()
{
	GetWorldTimerManager().ClearTimer(EventTimerHandle);
}

void AEventZone::StopSequence()
{
	if (EventSequence)
	{
		ULevelSequencePlayer* Player = EventSequence->GetSequencePlayer();
		if (Player && Player->IsPlaying())
		{
			Player->Stop();
		}
	}
}

void AEventZone::StartEvent()
{
	if (AFlash* Flash = Cast<AFlash>(UGameplayStatics::GetActorOfClass(GetWorld(), AFlash::StaticClass())))
	{
		//if (!Flash->bIsIrradiateEventZone)
		{
			if (EventSequence)
			{
				EventSequence->GetSequencePlayer()->Play();
			}
		}
	}
	
	GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Yellow, "StartEvent");
	
	GetWorldTimerManager().SetTimer(EventTimerHandle, this, &ThisClass::StartEvent, FMath::FRandRange(MinEventInterval, MaxEventInterval), false);
}

void AEventZone::OnSequenceEnded()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
}

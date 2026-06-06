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
		// Pause된 시퀀스는 IsPlaying()이 false이므로 가드 없이 Stop합니다.
		if (ULevelSequencePlayer* Player = EventSequence->GetSequencePlayer())
		{
			Player->Stop();
		}
	}
}

void AEventZone::OnFlashStopElapsed()
{
	// 후레쉬를 끕니다.
	if (AFlash* Flash = Cast<AFlash>(UGameplayStatics::GetActorOfClass(GetWorld(), AFlash::StaticClass())))
	{
		Flash->SetLightEnabled(false);
	}

	// Pause 상태인 이벤트 시퀀스를 정지합니다.
	StopSequence();
}

void AEventZone::StartEvent()
{
	if (CanStartEvent() && EventSequence)
	{
		EventSequence->GetSequencePlayer()->Play();
	}

	GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Yellow, "StartEvent");

	GetWorldTimerManager().SetTimer(EventTimerHandle, this, &ThisClass::StartEvent, FMath::FRandRange(MinEventInterval, MaxEventInterval), false);
}

bool AEventZone::CanStartEvent() const
{
	AFlash* Flash = Cast<AFlash>(UGameplayStatics::GetActorOfClass(GetWorld(), AFlash::StaticClass()));
	if (Flash && Flash->bIsIrradiateEventZone) return false;

	if (IsPlayerLookingAtZone()) return false;

	return true;
}

bool AEventZone::IsPlayerLookingAtZone() const
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return false;

	FVector ViewLocation;
	FRotator ViewRotation;
	PC->GetPlayerViewPoint(ViewLocation, ViewRotation);

	const FVector ToZone = (GetActorLocation() - ViewLocation).GetSafeNormal();
	const float DotProduct = FVector::DotProduct(ViewRotation.Vector(), ToZone);
	const float ThresholdCos = FMath::Cos(FMath::DegreesToRadians(PlayerLookAtAngleThreshold));

	return DotProduct >= ThresholdCos;
}

void AEventZone::OnFlashHit()
{
	if (!EventSequence) return;

	ULevelSequencePlayer* Player = EventSequence->GetSequencePlayer();
	if (!Player || !Player->IsPlaying()) return;

	// 이미 Stop 예약 중이면 중복 처리하지 않습니다.
	if (GetWorldTimerManager().IsTimerActive(FlashStopTimerHandle)) return;

	// 플레이어가 이 구역을 바라보고 있을 때만 Pause → 지연 Stop
	if (!IsPlayerLookingAtZone()) return;

	Player->Pause();
	GetWorldTimerManager().SetTimer(FlashStopTimerHandle, this, &ThisClass::OnFlashStopElapsed, FlashStopDelay, false);
}

void AEventZone::OnSequenceEnded()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
}

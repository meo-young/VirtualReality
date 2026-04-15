#include "EventManager.h"
#include "LevelSequencePlayer.h"
#include "VirtualReality.h"

void AEventManager::BeginPlay()
{
	Super::BeginPlay();

	StartEventCycle();
}

void AEventManager::StartEventCycle()
{
	if (ZoneEvents.IsEmpty()) return;

	// 첫 Entity 이벤트 발생 기준값을 랜덤 설정합니다.
	NextEntityEventThreshold = FMath::RandRange(MinEntityEventCycle, MaxEntityEventCycle);
	
	// MinEventInterval, MaxEventInterval 사이의 랜덤한 인터벌로 이벤트를 스폰합니다.
	ScheduleNextEvent();
}

void AEventManager::StopEventCycle()
{
	GetWorldTimerManager().ClearTimer(EventTimerHandle);

	LOG(TEXT("이벤트 사이클이 중단되었습니다."));
}

void AEventManager::ScheduleNextEvent()
{
	const float Interval = FMath::FRandRange(MinEventInterval, MaxEventInterval);
	GetWorldTimerManager().SetTimer(EventTimerHandle, this, &AEventManager::PlayNextEvent, Interval, false);
	LOG(TEXT("%f 후에 이벤트가 발생합니다."), Interval);
}

void AEventManager::PlayNextEvent()
{
	// 이벤트를 출력할 구역을 선정합니다. 감시 중인 구역만 남은 경우 다음 기회에 재시도 합니다.
	ACCTV* SelectedCCTV = PickEventZone();
	if (!SelectedCCTV) return;

	// Entity 턴이고 해당 구역에 Entity 이벤트가 존재할 때만 Entity 이벤트를 출력합니다.
	const bool bHasEntityEvents = !ZoneEvents[SelectedCCTV].EntityEvents.IsEmpty();
	if (IsEntityTurn() && bHasEntityEvents)
	{
		PlayEntityEvent(SelectedCCTV);
	}
	else
	{
		PlayNormalEvent(SelectedCCTV);
	}
}

void AEventManager::PlayEntityEvent(ACCTV* SelectedCCTV)
{
	// 선정된 구역의 랜덤한 Entity 이벤트를 선택합니다.
	FEventInfo& Zone = ZoneEvents[SelectedCCTV];
	const int32 EventIndex = FMath::RandRange(0, Zone.EntityEvents.Num() - 1);
	
	// 이벤트 출력 가중치를 더하고, PlayedEventsByZone에 이벤트 정보를 저장한 후 해당 이벤트를 출력 후 원본 배열에서 제거합니다.
	PlayedEventsByZone.Add(SelectedCCTV, Zone.EntityEvents[EventIndex]);
	Zone.EntityEvents[EventIndex]->GetSequencePlayer()->Play();
	Zone.EventPenaltyWeight += 1.f;
	Zone.EntityEvents.RemoveAt(EventIndex);

	// Entity 이벤트 활성 구역을 기록하고 다음 Entity 기준값을 갱신합니다.
	EntityEventActiveCCTV = SelectedCCTV;
	NextEntityEventThreshold = ++EventCallCount + FMath::RandRange(MinEntityEventCycle, MaxEntityEventCycle);

	LOG(TEXT("Entity 이벤트 %d를 재생합니다. (잔여: %d개)"), EventIndex, Zone.EntityEvents.Num());
	ScheduleNextEvent();
}

void AEventManager::PlayNormalEvent(ACCTV* SelectedCCTV)
{
	// 선정된 구역의 랜덤한 Normal 이벤트를 선택합니다.
	FEventInfo& Zone = ZoneEvents[SelectedCCTV];
	const int32 EventIndex = FMath::RandRange(0, Zone.NormalEvents.Num() - 1);

	// 이벤트 출력 가중치를 더하고, PlayedEventsByZone에 이벤트 정보를 저장한 후 해당 이벤트를 출력 후 원본 배열에서 제거합니다.
	PlayedEventsByZone.Add(SelectedCCTV, Zone.NormalEvents[EventIndex]);
	Zone.NormalEvents[EventIndex]->GetSequencePlayer()->Play();
	Zone.EventPenaltyWeight += 1.f;
	Zone.NormalEvents.RemoveAt(EventIndex);

	// 이벤트 총 출력 횟수를 더합니다.
	++EventCallCount;

	LOG(TEXT("Normal 이벤트 %d를 재생합니다. (잔여: %d개)"), EventIndex, Zone.NormalEvents.Num());
	ScheduleNextEvent();
}

void AEventManager::OnLeverReachedEnd()
{
	if (!WatchedCCTV) return;

	if (!PlayedEventsByZone.Contains(WatchedCCTV))
	{
		HandleNoPlayedEvents();
	}
	else
	{
		RestoreWatchedZoneState();
	}
}

void AEventManager::HandleNoPlayedEvents()
{
	LOG(TEXT("감시 중인 구역에서 발생한 이벤트가 없습니다."));
}

void AEventManager::RestoreWatchedZoneState()
{
	// 재생 중인 이벤트를 중단합니다.
	ALevelSequenceActor* PlayedEvent = PlayedEventsByZone[WatchedCCTV];
	if (IsValid(PlayedEvent))
	{
		ULevelSequencePlayer* Player = PlayedEvent->GetSequencePlayer();
		if (Player && Player->IsPlaying())
		{
			Player->Stop();
		}
	}

	// 이 구역에 Entity 이벤트가 있었다면 활성 상태를 해제합니다.
	if (EntityEventActiveCCTV == WatchedCCTV)
	{
		EntityEventActiveCCTV = nullptr;
	}

	PlayedEventsByZone.Remove(WatchedCCTV);

	LOG(TEXT("감시 중인 구역의 이벤트 장소 상태가 복원되었습니다."));
}

uint8 AEventManager::IsEntityTurn()
{
	return (EventCallCount >= NextEntityEventThreshold) && (EntityEventActiveCCTV == nullptr);
}

void AEventManager::OnMonitorChanged(ACCTV* InWatchedCCTV)
{
	// 현재 감시 중인 CCTV를 캐싱합니다.
	WatchedCCTV = InWatchedCCTV;
}

ACCTV* AEventManager::PickEventZone()
{
	// 이벤트를 출력할 수 있는 구역의 후보군을 선정합니다. 실제 이벤트 출력 로직이 아닙니다.
	TArray<TPair<ACCTV*, float>> Candidates;
	float TotalWeight = 0.f;
	for (auto& [CCTV, Zone] : ZoneEvents)
	{
		// 현재 감시 중인 구역 및 이벤트가 이미 출력된 구역은 후보군에서 제외합니다.
		if (CCTV == WatchedCCTV || PlayedEventsByZone.Contains(CCTV)) continue;

		const float Weight = 1.f / (1.f + Zone.EventPenaltyWeight);
		Candidates.Add({ CCTV.Get(), Weight });
		TotalWeight += Weight;
	}

	// 감시 중인 구역만 남은 경우 재시도합니다.
	if (Candidates.IsEmpty())
	{
		ScheduleNextEvent();
		return nullptr;
	}

	// 가중치 랜덤으로 실제 이벤트를 출력할 구역을 선택합니다.
	ACCTV* Selected = Candidates.Last().Key;
	const float Rand = FMath::FRandRange(0.f, TotalWeight);
	float Accumulated = 0.f;
	for (int32 i = 0; i < Candidates.Num() - 1; ++i)
	{
		Accumulated += Candidates[i].Value;
		if (Rand < Accumulated) { Selected = Candidates[i].Key; break; }
	}

	return Selected;
}


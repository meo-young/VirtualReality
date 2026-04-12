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

	// EventInterval 후 첫 번째 이벤트를 재생합니다.
	GetWorldTimerManager().SetTimer(EventTimerHandle, this, &AEventManager::PlayNextEvent, EventInterval, false);

	LOG(TEXT("이벤트 사이클이 시작되었습니다. (인터벌: %.1f초)"), EventInterval);
}

void AEventManager::StopEventCycle()
{
	GetWorldTimerManager().ClearTimer(EventTimerHandle);

	LOG(TEXT("이벤트 사이클이 중단되었습니다."));
}

void AEventManager::PlayNextEvent()
{
	ACCTV* SelectedCCTV = SelectZone();
	if (!SelectedCCTV) return;

	FEventInfo& Zone = ZoneEvents[SelectedCCTV];

	// Zone 내 랜덤 이벤트를 선택합니다.
	const int32 EventIndex = FMath::RandRange(0, Zone.Events.Num() - 1);
	ALevelSequenceActor* SelectedActor = Zone.Events[EventIndex];

	// 패널티 가중치를 누적하고 재생한 이벤트를 배열에서 제거합니다.
	Zone.EventPenaltyWeight += 1.f;
	Zone.Events.RemoveAt(EventIndex);

	// 레벨에 배치된 LevelSequenceActor의 플레이어를 직접 사용하여 재생합니다.
	ULevelSequencePlayer* Player = SelectedActor->GetSequencePlayer();
	Player->Play();

	LOG(TEXT("이벤트 %d를 재생합니다. (잔여: %d개)"), EventIndex, Zone.Events.Num());

	// 시퀀스 완료 후 EventInterval만큼 대기하고 다음 이벤트를 예약합니다.
	GetWorldTimerManager().SetTimer(EventTimerHandle, this, &AEventManager::PlayNextEvent, EventInterval, false);
}

void AEventManager::OnMonitorChangedDelegate(ACCTV* InWatchedCCTV)
{
	// 현재 감시 중인 CCTV를 캐싱합니다.
	WatchedCCTV = InWatchedCCTV;
}

ACCTV* AEventManager::SelectZone()
{
	// 후보 Zone(비어있지 않고 감시 중이지 않은)을 수집합니다.
	TArray<TPair<ACCTV*, float>> Candidates;
	float TotalWeight = 0.f;
	bool bAnyNonEmpty = false;

	for (auto& [CCTV, Zone] : ZoneEvents)
	{
		if (Zone.Events.IsEmpty()) continue;
		bAnyNonEmpty = true;
		if (CCTV == WatchedCCTV) continue;

		const float Weight = 1.f / (1.f + Zone.EventPenaltyWeight);
		Candidates.Add({ CCTV.Get(), Weight });
		TotalWeight += Weight;
	}

	// 비어있지 않은 Zone이 전혀 없으면 소진 신호를 반환합니다.
	if (!bAnyNonEmpty)
	{
		LOG(TEXT("모든 이벤트가 소진되었습니다. 이벤트 사이클을 중단합니다."));
		return nullptr;
	}

	// 유효한 후보가 없으면 감시 중인 구역만 남은 것이므로 나중에 재시도합니다.
	if (Candidates.IsEmpty())
	{
		GetWorldTimerManager().SetTimer(EventTimerHandle, this, &AEventManager::PlayNextEvent, EventInterval, false);
		LOG(TEXT("모든 구역이 감시 중입니다. %.1f초 후 다시 시도합니다."), EventInterval);
		return nullptr;
	}

	// 가중치 랜덤 선택. 마지막 항목은 폴백으로 두어 경계값 편향을 방지합니다.
	const float Rand = FMath::FRandRange(0.f, TotalWeight);
	float Accumulated = 0.f;
	for (int32 i = 0; i < Candidates.Num() - 1; ++i)
	{
		Accumulated += Candidates[i].Value;
		if (Rand < Accumulated) return Candidates[i].Key;
	}

	return Candidates.Last().Key;
}

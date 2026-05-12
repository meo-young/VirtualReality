#include "MainGameMode.h"
#include "EngineUtils.h"
#include "Actor/Monitor.h"
#include "Actor/EventZone.h"
#include "Manager/EventManager.h"
#include "VirtualReality.h"
#include "Actor/Grabbable/EntityClearLever/EntityClearLever.h"

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	bEventStarted = false;

	AMonitor* Monitor = nullptr;
	AEntityClearLever* EntityClearLever = nullptr;

	// 월드에서 필요한 액터들을 탐색합니다.
	for (AMonitor* Actor : TActorRange<AMonitor>(GetWorld()))
	{
		Monitor = Actor;
		break;
	}
	for (AEventManager* Actor : TActorRange<AEventManager>(GetWorld()))
	{
		CachedEventManager = Actor;
		break;
	}
	for (AEntityClearLever* Actor : TActorRange<AEntityClearLever>(GetWorld()))
	{
		EntityClearLever = Actor;
		break;
	}
	for (AEventZone* Zone : TActorRange<AEventZone>(GetWorld()))
	{
		CachedEventZones.Add(Zone);
	}

	// Monitor의 채널 전환 델리게이트에 EventManager와 GameMode를 바인딩합니다.
	if (Monitor && CachedEventManager)
	{
		Monitor->OnMonitorChangedDelegate.AddUObject(CachedEventManager, &AEventManager::OnMonitorChanged);
		Monitor->OnMonitorChangedDelegate.AddUObject(this, &ThisClass::OnMonitorChanged);
		LOG(TEXT("Monitor → EventManager 델리게이트 바인딩이 완료되었습니다."));
	}

	if (Monitor && EntityClearLever && CachedEventManager)
	{
		EntityClearLever->OnLeverReachedEndDelegate.AddUObject(Monitor, &AMonitor::OnLeverReachedEnd);
		EntityClearLever->OnLeverReachedEndDelegate.AddUObject(CachedEventManager, &AEventManager::OnLeverReachedEnd);
		LOG(TEXT("EntityClearLever → Monitor, EventManager 델리게이트 바인딩이 완료되었습니다."));
	}
}

void AMainGameMode::StartEvent()
{
	if (CachedEventManager)
	{
		CachedEventManager->StartEventCycle();
	}
	for (AEventZone* Zone : CachedEventZones)
	{
		Zone->StartEventCycle();
	}
}

void AMainGameMode::StopEvent()
{
	if (CachedEventManager)
	{
		CachedEventManager->StopEventCycle();
	}
	for (AEventZone* Zone : CachedEventZones)
	{
		Zone->StopEventCycle();
	}
}

void AMainGameMode::OnMonitorChanged(ACCTV* InCCTV)
{
	if (!bEventStarted)
	{
		bEventStarted = true;
		StartEvent();
	}
}

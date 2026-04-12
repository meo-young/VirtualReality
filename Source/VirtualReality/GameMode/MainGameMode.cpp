#include "MainGameMode.h"
#include "EngineUtils.h"
#include "Actor/Monitor.h"
#include "Manager/EventManager.h"
#include "VirtualReality.h"
#include "Actor/Grabbable/EntityClearLever/EntityClearLever.h"

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	AMonitor* Monitor = nullptr;
	AEventManager* EventManager = nullptr;
	AEntityClearLever* EntityClearLever = nullptr;

	// 월드에서 Monitor와 EventManager를 탐색합니다.
	for (AMonitor* Actor : TActorRange<AMonitor>(GetWorld()))
	{
		Monitor = Actor;
		break;
	}
	for (AEventManager* Actor : TActorRange<AEventManager>(GetWorld()))
	{
		EventManager = Actor;
		break;
	}
	for (AEntityClearLever* Actor : TActorRange<AEntityClearLever>(GetWorld()))
	{
		EntityClearLever = Actor;
		break;
	}

	// Monitor의 채널 전환 델리게이트에 EventManager를 바인딩합니다.
	if (Monitor && EventManager)
	{
		Monitor->OnMonitorChangedDelegate.AddUObject(EventManager, &AEventManager::OnMonitorChanged);
		LOG(TEXT("Monitor → EventManager 델리게이트 바인딩이 완료되었습니다."));
	}

	if (Monitor && EntityClearLever && EventManager)
	{
		EntityClearLever->OnLeverReachedEndDelegate.AddUObject(Monitor, &AMonitor::OnLeverReachedEnd);
		EntityClearLever->OnLeverReachedEndDelegate.AddUObject(EventManager, &AEventManager::OnLeverReachedEnd);
		LOG(TEXT("EntityClearLever → Monitor, EventManager 델리게이트 바인딩이 완료되었습니다."));
	}
	
}

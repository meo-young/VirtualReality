#include "MainGameMode.h"
#include "EngineUtils.h"
#include "Actor/Monitor.h"
#include "Manager/EventManager.h"
#include "VirtualReality.h"

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	AMonitor* Monitor = nullptr;
	AEventManager* EventManager = nullptr;

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

	// Monitor의 채널 전환 델리게이트에 EventManager를 바인딩합니다.
	if (Monitor && EventManager)
	{
		Monitor->OnMonitorChangedDelegate.AddUObject(EventManager, &AEventManager::OnMonitorChangedDelegate);
		LOG(TEXT("Monitor → EventManager 델리게이트 바인딩이 완료되었습니다."));
	}
	else
	{
		LOG(TEXT("Monitor 또는 EventManager를 찾을 수 없어 바인딩에 실패했습니다."));
	}
}

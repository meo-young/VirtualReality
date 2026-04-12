#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelSequenceActor.h"
#include "Actor/CCTV.h"
#include "EventManager.generated.h"

USTRUCT(BlueprintType)
struct FEventInfo
{
	GENERATED_BODY()

	/** 구역에 해당하는 이벤트 액터 목록입니다. 레벨에 배치된 LevelSequenceActor를 직접 참조합니다. */
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<ALevelSequenceActor>> Events;

	/** 이벤트가 발생한 횟수를 추적하는 변수입니다. 높을수록 이벤트가 발생할 확률이 낮아집니다. */
	float EventPenaltyWeight = 0.0f;
};

UCLASS()
class VIRTUALREALITY_API AEventManager : public AActor
{
	GENERATED_BODY()


// Lifecycle Section
public:
	virtual void BeginPlay() override;


// Member Function
public:
	/** 이벤트 사이클을 시작합니다. */
	UFUNCTION()
	void StartEventCycle();

	/** 이벤트 사이클을 중단합니다. */
	UFUNCTION()
	void StopEventCycle();

	/** EntityClearLever가 끝까지 내려졌을 때 호출되는 콜백입니다. */
	void OnLeverReachedEnd();

	/** Monitor의 채널 전환 델리게이트 콜백입니다. */
	void OnMonitorChanged(ACCTV* InWatchedCCTV);

private:
	void PlayNextEvent();

	/** 가중치 기반으로 다음 이벤트를 재생할 Zone의 CCTV를 반환합니다. 소진 시 nullptr을 반환합니다. */
	ACCTV* SelectZone();

	/** 감시 중인 구역에 발생한 이벤트가 없을 때 처리합니다. */
	void HandleNoPlayedEvents();

	/** 감시 중인 구역에서 재생된 이벤트의 장소 상태를 복원합니다. */
	void RestoreWatchedZoneState(FEventInfo& PlayedInfo);


// Variable Section
protected:
	/** CCTV를 키로, 해당 구역의 이벤트 정보를 값으로 관리합니다. */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "변수|이벤트")
	TMap<TObjectPtr<ACCTV>, FEventInfo> ZoneEvents;

	/** 이벤트 재생 완료 후 다음 이벤트까지의 대기 시간(초)입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|이벤트")
	float EventInterval = 5.0f;

private:
	FTimerHandle EventTimerHandle;

	/** 현재 Monitor가 감시 중인 CCTV입니다. 해당 구역의 이벤트는 선택에서 제외됩니다. */
	UPROPERTY()
	TObjectPtr<ACCTV> WatchedCCTV;

	/** Zone별로 재생된 이벤트 목록입니다. 장소 상태 복원 대상 추적에 사용됩니다. */
	UPROPERTY()
	TMap<TObjectPtr<ACCTV>, FEventInfo> PlayedEventsByZone;

};

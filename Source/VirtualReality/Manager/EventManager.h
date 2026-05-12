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
	
	/** 구역에 해당하는 Entity 이벤트 액터 목록입니다. 레벨에 배치된 LevelSequenceActor를 직접 참조합니다. */
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<ALevelSequenceActor>> EntityEvents;

	/** 구역에 해당하는 일반 이벤트 액터 목록입니다. 레벨에 배치된 LevelSequenceActor를 직접 참조합니다. */
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<ALevelSequenceActor>> NormalEvents;

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
	void StartEventCycle();

	/** 이벤트 사이클을 중단합니다. */
	void StopEventCycle();

	/** EntityClearLever가 끝까지 내려졌을 때 호출되는 콜백입니다. */
	void OnLeverReachedEnd();

	/** Monitor의 채널 전환 델리게이트 콜백입니다. */
	void OnMonitorChanged(ACCTV* InWatchedCCTV);

private:
	void PlayNextEvent();

	/** 랜덤 인터벌로 다음 이벤트 타이머를 예약합니다. */
	void ScheduleNextEvent();

	/** 감시 중인 구역에 발생한 이벤트가 없을 때 처리합니다. */
	void HandleNoPlayedEvents();

	/** 감시 중인 구역에서 재생된 이벤트의 장소 상태를 복원합니다. */
	void RestoreWatchedZoneState();

	/** 현재가 Entity 이벤트를 출력할 턴인지 여부를 반환하는 함수입니다. */
	uint8 IsEntityTurn();

	/** 이벤트를 출력할 Zone을 가중치 기반으로 선정하여 반환합니다. 감시 중인 구역만 남은 경우 재시도 후 nullptr을 반환합니다. */
	ACCTV* PickEventZone();

	/** 선정된 구역에 Entity 이벤트를 출력합니다. */
	void PlayEntityEvent(ACCTV* SelectedCCTV);

	/** 선정된 구역에 Normal 이벤트를 출력합니다. */
	void PlayNormalEvent(ACCTV* SelectedCCTV);

	/** 플레이어 사망 처리를 수행합니다. */
	void PlayerDeath();


// Variable Section
protected:
	/** CCTV를 키로, 해당 구역의 이벤트 정보를 값으로 관리합니다. */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "변수|이벤트")
	TMap<TObjectPtr<ACCTV>, FEventInfo> ZoneEvents;

	/** 다음 이벤트까지 대기 시간의 최솟값(초)입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|이벤트")
	float MinEventInterval = 30.0f;

	/** 다음 이벤트까지 대기 시간의 최댓값(초)입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|이벤트")
	float MaxEventInterval = 50.0f;

	/** Entity 이벤트 발생까지 필요한 최소 이벤트 횟수입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|이벤트")
	int32 MinEntityEventCycle = 3;

	/** Entity 이벤트 발생까지 필요한 최대 이벤트 횟수입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|이벤트")
	int32 MaxEntityEventCycle = 6;

	/** Entity 이벤트 미스캔 시 사망까지의 제한 시간(초)입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|사망")
	float EntityEventDeathTimeout = 30.0f;

private:
	FTimerHandle EventTimerHandle;
	FTimerHandle EntityEventDeathTimerHandle;

	/** 스캔되지 않은 NormalEvent의 누적 개수입니다. */
	int32 UnscannedNormalEventCount = 0;

	/** 총 이벤트 재생 횟수를 추적합니다. */
	int32 EventCallCount = 0;

	/** 다음 Entity 이벤트가 발생할 EventCallCount 기준값입니다. */
	int32 NextEntityEventThreshold = 0;

	/** 현재 Monitor가 감시 중인 CCTV입니다. 해당 구역의 이벤트는 선택에서 제외됩니다. */
	UPROPERTY()
	TObjectPtr<ACCTV> WatchedCCTV;

	/** Zone별로 현재 재생 중인 이벤트입니다. */
	TMap<TObjectPtr<ACCTV>, TObjectPtr<ALevelSequenceActor>> PlayedEventsByZone;

	/** Entity 이벤트가 활성화된 구역입니다. nullptr이면 Entity 이벤트가 없는 상태입니다. */
	UPROPERTY()
	TObjectPtr<ACCTV> EntityEventActiveCCTV;

};

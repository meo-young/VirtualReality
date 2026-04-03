#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CCTVManager.generated.h"

class ACCTV;

UCLASS()
class VIRTUALREALITY_API UCCTVManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	/** CCTV를 관리 목록에 등록합니다. */
	void RegisterCCTV(ACCTV* CCTV);

	/** CCTV를 관리 목록에서 해제합니다. */
	void UnregisterCCTV(ACCTV* CCTV);

	/** 지정한 인덱스의 CCTV를 활성화하고 나머지는 비활성화합니다. */
	void SetActiveCCTV(int32 Index);

	/** 모든 CCTV의 캡처를 비활성화합니다. */
	void DeactivateAllCCTVs();

public:
	/** 등록된 CCTV 목록을 반환합니다. */
	FORCEINLINE const TArray<TObjectPtr<ACCTV>>& GetRegisteredCCTVs() const { return RegisteredCCTVs; }

	/** 현재 활성화된 CCTV의 인덱스를 반환합니다. -1이면 활성화된 CCTV가 없습니다. */
	FORCEINLINE int32 GetActiveCCTVIndex() const { return ActiveCCTVIndex; }

private:
	/** 등록된 CCTV 목록입니다. */
	TArray<TObjectPtr<ACCTV>> RegisteredCCTVs;

	/** 현재 활성화된 CCTV의 인덱스입니다. */
	int32 ActiveCCTVIndex = -1;
};
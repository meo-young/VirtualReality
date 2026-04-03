#include "CCTVManager.h"
#include "Actor/CCTV.h"
#include "VirtualReality.h"

void UCCTVManager::RegisterCCTV(ACCTV* CCTV)
{
	if (!CCTV) return;

	RegisteredCCTVs.AddUnique(CCTV);

	// 새로 등록된 CCTV는 기본적으로 캡처를 비활성화합니다.
	CCTV->SetCaptureEnabled(false);

	LOG(TEXT("CCTV가 등록되었습니다. 총 등록된 CCTV 수: %d"), RegisteredCCTVs.Num());
}

void UCCTVManager::UnregisterCCTV(ACCTV* CCTV)
{
	if (!CCTV) return;

	const int32 RemovedIndex = RegisteredCCTVs.Find(CCTV);
	RegisteredCCTVs.Remove(CCTV);

	// 해제된 CCTV가 활성 CCTV였다면 활성 인덱스를 초기화합니다.
	if (ActiveCCTVIndex == RemovedIndex)
	{
		ActiveCCTVIndex = -1;
	}

	LOG(TEXT("CCTV가 해제되었습니다. 총 등록된 CCTV 수: %d"), RegisteredCCTVs.Num());
}

void UCCTVManager::SetActiveCCTV(int32 Index)
{
	if (!RegisteredCCTVs.IsValidIndex(Index))
	{
		LOG(TEXT("유효하지 않은 CCTV 인덱스입니다: %d"), Index);
		return;
	}

	// 이전 활성 CCTV를 비활성화합니다.
	if (RegisteredCCTVs.IsValidIndex(ActiveCCTVIndex))
	{
		RegisteredCCTVs[ActiveCCTVIndex]->SetCaptureEnabled(false);
	}

	// 새 CCTV를 활성화합니다.
	ActiveCCTVIndex = Index;
	RegisteredCCTVs[ActiveCCTVIndex]->SetCaptureEnabled(true);

	LOG(TEXT("활성 CCTV가 인덱스 %d로 변경되었습니다."), ActiveCCTVIndex);
}

void UCCTVManager::DeactivateAllCCTVs()
{
	for (TObjectPtr<ACCTV>& CCTV : RegisteredCCTVs)
	{
		if (CCTV)
		{
			CCTV->SetCaptureEnabled(false);
		}
	}

	ActiveCCTVIndex = -1;

	LOG(TEXT("모든 CCTV가 비활성화되었습니다."));
}
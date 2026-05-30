#pragma once

#include "CoreMinimal.h"
#include "Abstract/BaseGameMode.h"
#include "MainGameMode.generated.h"

class ACCTV;
class AEventManager;
class AEventZone;

UCLASS()
class VIRTUALREALITY_API AMainGameMode : public ABaseGameMode
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void StartEvent();
	void StopEvent();

private:
	UPROPERTY()
	TObjectPtr<AEventManager> CachedEventManager;

	UPROPERTY()
	TArray<TObjectPtr<AEventZone>> CachedEventZones;
};

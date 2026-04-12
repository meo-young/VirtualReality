#pragma once

#include "CoreMinimal.h"
#include "Abstract/BaseGameMode.h"
#include "MainGameMode.generated.h"

UCLASS()
class VIRTUALREALITY_API AMainGameMode : public ABaseGameMode
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
};

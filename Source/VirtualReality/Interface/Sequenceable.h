#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Sequenceable.generated.h"

UINTERFACE(NotBlueprintable)
class USequenceable : public UInterface
{
	GENERATED_BODY()
};

class VIRTUALREALITY_API ISequenceable
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	virtual void OnSequenceStarted() = 0;
	
	UFUNCTION()
	virtual void OnSequenceEnded() = 0;
	
protected:
	void PlaySequence();
	
private:
	uint8 bIsBound : 1 = false;
	
};

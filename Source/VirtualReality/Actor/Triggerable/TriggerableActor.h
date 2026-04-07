#pragma once

#include "CoreMinimal.h"
#include "Actor/VRActorBase.h"
#include "TriggerableActor.generated.h"

class UBoxComponent;

UCLASS()
class VIRTUALREALITY_API ATriggerableActor : public AVRActorBase
{
	GENERATED_BODY()
	
public:
	ATriggerableActor();
	virtual void BeginPlay() override;

protected:
	/** 트리거가 발동될 때 호출되는 함수입니다. */
	virtual void OnTriggered();
	
private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<UBoxComponent> TriggerRegion;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|수치")
	float TriggerSpeedThreshold;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|상태")
	uint8 bIsTriggerEnabled : 1 = true;

};

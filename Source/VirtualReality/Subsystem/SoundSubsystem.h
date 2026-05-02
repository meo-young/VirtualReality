#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SoundSubsystem.generated.h"

class USoundCue;
class UAudioComponent;

UCLASS()
class VIRTUALREALITY_API USoundSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

// Lifecycle Section
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;


// Member Function
public:
	UAudioComponent* Play2D(USoundCue* Sound);
	UAudioComponent* PlayAtLocation(USoundCue* Sound, const FVector& Location);
	void Stop(UAudioComponent* AudioComponent);

private:
	UAudioComponent* AcquireComponent();
	
	UFUNCTION()
	void OnAudioFinished(UAudioComponent* Component);
	

// Variable Section
private:
	// 모든 풀 컴포넌트의 GC 루트입니다. TQueue는 UPROPERTY를 지원하지 않으므로 별도로 관리합니다.
	UPROPERTY()
	TArray<TObjectPtr<UAudioComponent>> AllComponents;

	UPROPERTY()
	TArray<TObjectPtr<UAudioComponent>> ActiveComponents;

	TQueue<UAudioComponent*> FreeList;

	static constexpr int32 PoolSize = 50;

};

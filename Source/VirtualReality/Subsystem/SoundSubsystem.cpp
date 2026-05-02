#include "SoundSubsystem.h"
#include "Components/AudioComponent.h"
#include "Components/SceneComponent.h"
#include "Sound/SoundCue.h"
#include "VirtualReality.h"

void USoundSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	AllComponents.Reserve(PoolSize);
	ActiveComponents.Reserve(PoolSize);

	for (int32 i = 0; i < PoolSize; i++)
	{
		if (UAudioComponent* AC = NewObject<UAudioComponent>())
		{
			AC->bAutoActivate = false;
			AC->RegisterComponent();
			AC->OnAudioFinishedNative.AddUObject(this, &ThisClass::OnAudioFinished);

			AllComponents.Add(AC);
			FreeList.Enqueue(AC);	
		}
	}
}

void USoundSubsystem::Deinitialize()
{
	AllComponents.Empty();
	ActiveComponents.Empty();

	Super::Deinitialize();
}

UAudioComponent* USoundSubsystem::Play2D(USoundCue* Sound)
{
	if (!Sound) return nullptr;

	UAudioComponent* AC = AcquireComponent();
	if (!AC) return nullptr;

	AC->SetSound(Sound);
	AC->bAllowSpatialization = false;
	AC->bOverrideAttenuation = false;
	AC->Play();
	return AC;
}

UAudioComponent* USoundSubsystem::PlayAtLocation(USoundCue* Sound, const FVector& Location)
{
	if (!Sound) return nullptr;

	UAudioComponent* AC = AcquireComponent();
	if (!AC) return nullptr;

	AC->SetSound(Sound);
	AC->bAllowSpatialization = true;
	AC->bOverrideAttenuation = false;
	AC->SetWorldLocation(Location);
	AC->Play();
	return AC;
}

void USoundSubsystem::Stop(UAudioComponent* AudioComponent)
{
	if (!AudioComponent) return;
	OnAudioFinished(AudioComponent);
}

UAudioComponent* USoundSubsystem::AcquireComponent()
{
	UAudioComponent* AC = nullptr;
	if (FreeList.Dequeue(AC))
	{
		ActiveComponents.Add(AC);
		return AC;
	}

	return nullptr;
}

void USoundSubsystem::OnAudioFinished(UAudioComponent* Component)
{
	if (!Component || !ActiveComponents.Contains(Component)) return;

	Component->Stop();
	Component->SetSound(nullptr);
	ActiveComponents.Remove(Component);
	FreeList.Enqueue(Component);
}

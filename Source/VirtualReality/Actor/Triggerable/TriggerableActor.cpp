#include "TriggerableActor.h"
#include "VirtualReality.h"
#include "Component/VRHapticComponent.h"
#include "Components/BoxComponent.h"
#include "Player/VRHand.h"


ATriggerableActor::ATriggerableActor()
{
	TriggerRegion = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerRegion"));
	TriggerRegion->SetupAttachment(Mesh);
}

void ATriggerableActor::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerRegion->OnComponentBeginOverlap.AddDynamic(this, &ATriggerableActor::OnOverlapBegin);
}

void ATriggerableActor::OnTriggered()
{
}

void ATriggerableActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsTriggerEnabled) return;
	
	if (!OtherActor || OtherActor == this) return;
	
	if (AVRHand* Hand = Cast<AVRHand>(OtherActor))
	{
		Hand->GetHapticComponent()->PlayHapticBurst(BurstHapticFrequency, BurstHapticAmplitude, BurstHapticDuration);
		OnTriggered();
	}
}

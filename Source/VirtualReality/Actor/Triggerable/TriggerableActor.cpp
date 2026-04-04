#include "TriggerableActor.h"
#include "VirtualReality.h"
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
	if (!OtherActor || OtherActor == this) return;
	
	if (AVRHand* Hand = Cast<AVRHand>(OtherActor))
	{
		if (FMath::Abs(Hand->GetHandVelocity().Z) >= 300.0f)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Overlap with: %s"), *OtherActor->GetName()));
			OnTriggered();
		}
	}
}

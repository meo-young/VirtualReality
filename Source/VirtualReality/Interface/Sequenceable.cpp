#include "Sequenceable.h"
#include "ActorSequenceComponent.h"
#include "ActorSequencePlayer.h"
#include "VirtualReality.h"

void ISequenceable::PlaySequence()
{
	AActor* Owner = Cast<AActor>(this);
	if (!Owner)
	{
		LOG(TEXT("Sequenceable는 AActor에만 적용 가능합니다. 함수를 종료합니다."));	
		return;
	}
	
	UActorSequenceComponent* SeqComp = Owner->FindComponentByClass<UActorSequenceComponent>();
	if (!SeqComp)
	{
		LOG(TEXT("ActorSequence가 존재하지 않습니다."));
		return;
	}

	UActorSequencePlayer* Player = SeqComp->GetSequencePlayer();
	if (!Player) return;
	
	if (!bIsBound)
	{
		Player->OnPlay.AddDynamic(this, &ThisClass::OnSequenceStarted);
		Player->OnFinished.AddDynamic(this, &ThisClass::OnSequenceEnded);
		bIsBound = true;
	}
	
	Player->Play();
}

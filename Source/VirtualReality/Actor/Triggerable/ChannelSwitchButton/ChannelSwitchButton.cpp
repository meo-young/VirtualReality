#include "ChannelSwitchButton.h"
#include "VirtualReality.h"
#include "Actor/Monitor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AChannelSwitchButton::AChannelSwitchButton()
{
	// ButtonMesh를 초기화합니다.
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>("ButtonMesh");
	ButtonMesh->SetupAttachment(Mesh);
	
	// BoxComponent를 ButtonMesh에 부착합니다.
	TriggerRegion->SetupAttachment(ButtonMesh);
}

void AChannelSwitchButton::BeginPlay()
{
	Super::BeginPlay();
	
	Monitor = Cast<AMonitor>(UGameplayStatics::GetActorOfClass(GetWorld(), AMonitor::StaticClass()));
}

void AChannelSwitchButton::OnTriggered()
{
	Super::OnTriggered();
	
	GetWorldTimerManager().SetTimer(ChannelSwitchTimerHandle, this, &ThisClass::SwitchChannel, 0.2f, false);
	PlaySequence();
}

void AChannelSwitchButton::SwitchChannel()
{
	Monitor->SwitchToNextCCTV();
}

void AChannelSwitchButton::OnSequenceStarted()
{
	bIsTriggerEnabled = false;
}

void AChannelSwitchButton::OnSequenceEnded()
{
	bIsTriggerEnabled = true;
}

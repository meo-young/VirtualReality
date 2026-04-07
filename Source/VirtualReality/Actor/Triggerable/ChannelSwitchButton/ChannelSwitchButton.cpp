#include "ChannelSwitchButton.h"
#include "VirtualReality.h"
#include "Actor/Monitor.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"

AChannelSwitchButton::AChannelSwitchButton()
{
	// ButtonMesh를 초기화합니다.
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>("ButtonMesh");
	ButtonMesh->SetupAttachment(Mesh);
	
	// ChannelText를 초기화합니다.
	ChannelText = CreateDefaultSubobject<UTextRenderComponent>("ChannelText");
	ChannelText->SetupAttachment(ButtonMesh);
	ChannelText->SetRelativeLocation(FVector(86.153847f, 0.0f, 141.354167f));
	ChannelText->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	ChannelText->SetRelativeScale3D(FVector(6.153846f, 6.153846f, 26.666666f));
	ChannelText->SetHorizontalAlignment(EHTA_Center);
	ChannelText->SetText(FText::FromString(FString::Printf(TEXT("%d"), ChannelNum)));
	
	// BoxComponent를 ButtonMesh에 부착합니다.
	TriggerRegion->SetupAttachment(ButtonMesh);
}

void AChannelSwitchButton::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	ChannelText->SetText(FText::FromString(FString::Printf(TEXT("%d"), ChannelNum)));
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
	if (!Monitor)
	{
		LOG(TEXT("Monitor에 대한 참조가 정상적이지 않습니다."))
		return;
	}
	
	Monitor->SwitchToCCTV(ChannelNum-1);
}

void AChannelSwitchButton::OnSequenceStarted()
{
	bIsTriggerEnabled = false;
}

void AChannelSwitchButton::OnSequenceEnded()
{
	bIsTriggerEnabled = true;
}

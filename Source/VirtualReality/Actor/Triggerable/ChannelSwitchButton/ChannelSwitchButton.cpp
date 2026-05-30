#include "ChannelSwitchButton.h"
#include "VirtualReality.h"
#include "Actor/Clock.h"
#include "Actor/Monitor.h"
#include "Components/BoxComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/SoundSubsystem.h"

AChannelSwitchButton::AChannelSwitchButton()
{
	// ButtonMesh를 초기화합니다.
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	ButtonMesh->SetupAttachment(Mesh);
	ButtonMesh->SetRelativeLocation(FVector(-50.0f, 0.0f, 7.0f));
	ButtonMesh->SetRelativeScale3D(FVector(0.33f, 0.425f, 0.4f));
	
	// BoxComponent를 ButtonMesh에 부착합니다.
	TriggerRegion->SetupAttachment(ButtonMesh);
	
	// TextRenderComponent를 초기화합니다. Font, Material, Color는 에디터에서 지정해야 합니다.
	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
	TextRenderComponent->SetupAttachment(Mesh);
	TextRenderComponent->SetText(FText::FromString(TEXT("NEXT CH")));
	TextRenderComponent->SetRelativeLocation(FVector(36.0f, 0.0f, 22.0f));
	TextRenderComponent->SetRelativeRotation(FRotator(90.0f, 90.0f, 0.0f));
	TextRenderComponent->XScale = 0.25f;
	TextRenderComponent->YScale = 0.3f;
	TextRenderComponent->HorizontalAlignment = EHTA_Center;
	TextRenderComponent->VerticalAlignment = EVRTA_TextCenter;
	
	// Button의 Emissive Light 역할을 수행하는 SpotLightComponent를 초기화합니다. Color는 에디터에서 지정해야 합니다.
	ButtonLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Button Light"));
	ButtonLight->SetupAttachment(ButtonMesh);
	ButtonLight->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	ButtonLight->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	ButtonLight->IntensityUnits = ELightUnits::Unitless;
	ButtonLight->SetIntensity(100.0f);
	ButtonLight->AttenuationRadius = 30.0f;
	ButtonLight->InnerConeAngle = 0.0f;
	ButtonLight->OuterConeAngle = 44.0f;
}

void AChannelSwitchButton::BeginPlay()
{
	Super::BeginPlay();

	Monitor = Cast<AMonitor>(UGameplayStatics::GetActorOfClass(GetWorld(), AMonitor::StaticClass()));
	Clock = Cast<AClock>(UGameplayStatics::GetActorOfClass(GetWorld(), AClock::StaticClass()));
}

void AChannelSwitchButton::OnTriggered()
{
	Super::OnTriggered();

	USoundSubsystem::Get(this).PlaySFXAtLocationByName(TEXT("Button"), GetActorLocation());
	GetWorldTimerManager().SetTimer(ChannelSwitchTimerHandle, this, &ThisClass::SwitchChannel, 0.2f, false);
	PlaySequence();

	// 첫 트리거 시점에 Clock을 가동합니다.
	if (!bIsClockStarted && Clock)
	{
		bIsClockStarted = true;
		Clock->ActivateTimer();
	}
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

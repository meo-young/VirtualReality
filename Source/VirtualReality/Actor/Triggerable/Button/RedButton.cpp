#include "RedButton.h"

ARedButton::ARedButton()
{
	// 버튼 메시 컴포넌트를 생성하고 루트에 부착합니다.
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Button"));
	ButtonMesh->SetupAttachment(Mesh);
}

void ARedButton::OnSequenceStarted()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Red Button Sequence Started"));
}

void ARedButton::OnSequenceEnded()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Red Button Sequence Ended"));
}

void ARedButton::OnTriggered()
{
	Super::OnTriggered();
	
	PlaySequence();
}

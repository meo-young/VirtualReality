#include "MainPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "VirtualReality.h"

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	// Enhanced Input 서브시스템에 입력 매핑 컨텍스트를 등록합니다.
	ULocalPlayer* LP = GetLocalPlayer();
	if (!LP) return;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!Subsystem) return;

	if (IMC)
	{
		Subsystem->AddMappingContext(IMC, 0);
		LOG(TEXT("IMC 입력 컨텍스트가 등록되었습니다."));
	}
	else
	{
		LOG(TEXT("IMC가 설정되지 않았습니다. 블루프린트에서 할당해 주세요."));
	}
}

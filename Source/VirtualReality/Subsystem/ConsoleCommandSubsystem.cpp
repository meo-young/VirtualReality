#include "ConsoleCommandSubsystem.h"

void UConsoleCommandSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	// 콘솔 커맨드를 실행합니다.
	if (GEngine)
	{
		// FPS 통계 표시를 활성화합니다.
		GEngine->Exec(GetWorld(), TEXT("Stat FPS"));
		
	}
	
	// 최대 FPS를 200으로 설정합니다.
	IConsoleManager::Get().FindConsoleVariable(TEXT("t.MaxFPS"))->Set(999);

	// 렌더링 해상도 비율을 75%로 설정합니다. (값이 낮을수록 성능은 좋아지지만 화질은 떨어집니다)
	IConsoleManager::Get().FindConsoleVariable(TEXT("r.ScreenPercentage"))->Set(75);

	// 씬 렌더 타겟 리사이즈 방식을 설정합니다. (2: 항상 렌더 타겟을 현재 해상도에 맞게 리사이즈)
	IConsoleManager::Get().FindConsoleVariable(TEXT("r.SceneRenderTargetResizeMethod"))->Set(2);

}

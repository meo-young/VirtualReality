#pragma once

#include "CoreMinimal.h"
#include "Abstract/PlayerControllerBase.h"
#include "MainPlayerController.generated.h"

class UInputMappingContext;

UCLASS()
class VIRTUALREALITY_API AMainPlayerController : public APlayerControllerBase
{
	GENERATED_BODY()
	
public:
	virtual void SetupInputComponent() override;

private:
	/** Enhanced Input 서브시스템에 등록할 입력 매핑 컨텍스트입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> IMC;
	
};

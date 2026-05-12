#pragma once

#include "CoreMinimal.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/Actor.h"
#include "CCTV.generated.h"

class UCameraComponent;
class USceneCaptureComponent2D;
class UTextureRenderTarget2D;

UCLASS()
class VIRTUALREALITY_API ACCTV : public AActor
{
	GENERATED_BODY()

public:
	ACCTV();
	virtual void BeginPlay() override;
	
// Member Function
public:
	/** SceneCaptureComponent의 캡처 활성화 여부를 설정합니다. */
	void SetCaptureEnabled(bool bEnabled);
	
	
// Component Section	
protected:
	/** 씬 캡쳐 컴포넌트가 어떤 장면을 캡쳐하는지 볼 수 있게 하는 카메라입니다. 이 카메라는 씬 캡처 컴포넌트와 동일한 위치에 배치되어야 합니다. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수")
	TObjectPtr<UCameraComponent> FakeCameraComponent;

	/** 씬 캡처 컴포넌트입니다. 이 컴포넌트가 장면을 캡처하여 텍스처로 렌더링합니다. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수")
	TObjectPtr<USceneCaptureComponent2D> SceneCaptureComponent;
	
	

	
// Getter, Setter Section
public:
	FORCEINLINE UTextureRenderTarget2D* GetRenderTarget() const { return SceneCaptureComponent ? SceneCaptureComponent->TextureTarget : nullptr; };
};
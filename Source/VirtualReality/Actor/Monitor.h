#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Monitor.generated.h"

class URectLightComponent;
class UStaticMeshComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;
class UInputAction;

UCLASS()
class VIRTUALREALITY_API AMonitor : public AActor
{
	GENERATED_BODY()

public:
	AMonitor();
	virtual void BeginPlay() override;

protected:
	/** 모니터 화면을 표현하는 스태틱 메시입니다. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수")
	TObjectPtr<USceneComponent> MonitorRoot;
	
	/** 모니터 화면을 표현하는 스태틱 메시입니다. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수")
	TObjectPtr<UStaticMeshComponent> MonitorMesh;
	
	/** 모니터 화면을 표현하는 스태틱 메시입니다. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수")
	TObjectPtr<UStaticMeshComponent> ScreenMesh;

	/** 렌더 타겟을 표시할 화면 머티리얼입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	TObjectPtr<UMaterialInterface> ScreenMaterial;
	
	/** 모니터 화면의 빛을 나타내는 RectLight입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	TObjectPtr<URectLightComponent> ScreenLight;

	/** 렌더 타겟 텍스처를 연결할 머티리얼 파라미터 이름입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	FName RenderTargetParameterName = TEXT("RenderTarget");

	/** 다음 CCTV로 전환하는 입력 액션입니다. (기본 키: 2) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	TObjectPtr<UInputAction> IA_NextCCTV;

	/** 이전 CCTV로 전환하는 입력 액션입니다. (기본 키: 1) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	TObjectPtr<UInputAction> IA_PrevCCTV;

private:
	/** 화면에 적용할 다이나믹 머티리얼 인스턴스입니다. */
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> ScreenMaterialInstance;

public:
	/** 지정한 인덱스의 CCTV로 전환하고, 해당 렌더 타겟을 화면에 표시합니다. */
	UFUNCTION(BlueprintCallable, Category = "CCTV")
	void SwitchToCCTV(int32 Index);

	/** 현재 표시 중인 CCTV 인덱스를 반환합니다. */
	FORCEINLINE int32 GetCurrentCCTVIndex() const { return CurrentCCTVIndex; }

private:
	/** 다음 CCTV로 전환합니다. */
	void OnNextCCTV();

	/** 이전 CCTV로 전환합니다. */
	void OnPrevCCTV();

	/** 현재 표시 중인 CCTV의 인덱스입니다. */
	int32 CurrentCCTVIndex = -1;
};
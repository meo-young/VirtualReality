#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Monitor.generated.h"

class UPointLightComponent;
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
	
public:
	/** 지정한 인덱스의 CCTV로 전환하고, 해당 렌더 타겟을 화면에 표시합니다. */
	UFUNCTION(BlueprintCallable)
	void SwitchToCCTV(int32 Index);
	
private:
	/** 다음 CCTV로 전환합니다. */
	void OnNextCCTV();

	/** 이전 CCTV로 전환합니다. */
	void OnPrevCCTV();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<USceneComponent> MonitorRoot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> MonitorMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> ScreenMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<UPointLightComponent> ScreenLight;
	
protected:
	/** 렌더 타겟을 표시할 화면 머티리얼입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	TObjectPtr<UMaterialInterface> ScreenMaterial;

	/** 렌더 타겟 텍스처를 연결할 머티리얼 파라미터 이름입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	FName RenderTargetParameterName = TEXT("RenderTarget");

private:
	/** 화면에 적용할 다이나믹 머티리얼 인스턴스입니다. */
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> ScreenMaterialInstance;

	/** 현재 표시 중인 CCTV의 인덱스입니다. */
	int32 CurrentCCTVIndex = -1;
	
public:
	/** 현재 표시 중인 CCTV 인덱스를 반환합니다. */
	FORCEINLINE int32 GetCurrentCCTVIndex() const { return CurrentCCTVIndex; }
	
	
};
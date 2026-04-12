#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Monitor.generated.h"

class ACCTV;
class URectLightComponent;
class UStaticMeshComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;

DECLARE_MULTICAST_DELEGATE(FOnMonitorChangedDelegate);

UCLASS()
class VIRTUALREALITY_API AMonitor : public AActor
{
	GENERATED_BODY()

	
// Lifecycle Section	
public:
	AMonitor();
	virtual void BeginPlay() override;
	
	
// Delegate Section	
public:
	FOnMonitorChangedDelegate OnMonitorChangedDelegate;
	
	
// Member Function	
public:
	UFUNCTION(BlueprintCallable)
	void SwitchToNextCCTV();

private:
	void CollectCCTVs();
	void SetActiveCCTV(int32 Index);
	void DeactivateAllCCTVs();
	void ApplyNextCCTV();

	
// Component Section	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<USceneComponent> MonitorRoot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> MonitorMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> ScreenMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<URectLightComponent> ScreenRectLight;
	
	
// Variable Section	
protected:
	/** 렌더 타겟을 표시할 화면 머티리얼입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	TObjectPtr<UMaterialInterface> ScreenMaterial;

	/** 렌더 타겟 텍스처를 연결할 머티리얼 파라미터 이름입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	FName RenderTargetParameterName = TEXT("RenderTarget");
	
	/** CCTV 전환 시 출력할 노이즈 파라미터 이름입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	FName NoiseParameterName = TEXT("TV.Noise.Power");
	
	/** 노이즈 출력 시 모니터에 적용할 텍스처입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	UTextureRenderTarget2D* BlackRenderTarget = nullptr;
	
	/** 채널 전환 시 노이즈를 적용할 시간입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float NoiseEffectDuration = 0.5f;

private:
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> ScreenMaterialInstance;

	TArray<TObjectPtr<ACCTV>> RegisteredCCTVs;
	int32 ActiveCCTVIndex = -1;
	int32 PendingCCTVIndex = -1;
	FTimerHandle SwitchTimerHandle;


// Getter, Setter Section
public:
	FORCEINLINE int32 GetActiveCCTVIndex() const { return ActiveCCTVIndex; }
	
};
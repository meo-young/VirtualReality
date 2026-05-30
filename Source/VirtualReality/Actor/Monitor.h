#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Monitor.generated.h"

class ACCTV;
class URectLightComponent;
class UStaticMeshComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;
class UAudioComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMonitorChangedDelegate, ACCTV*);

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
	
	void OnLeverReachedEnd();
	
	UFUNCTION(BlueprintCallable)
	void HandlePlayerDeath();

private:
	void SetActiveCCTV(bool bIsEnable);
	void DeactivateAllCCTVs();
	void ApplyNextCCTV();
	void SetScreenMaterial(const float InNoisePower, const float InNoiseIntensity, UTextureRenderTarget2D* InRenderTarget);
	void RestoreScreenMaterial();
	void StartCCTVNoise();

	
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|Screen")
	TObjectPtr<UMaterialInterface> ScanningMaterial;
	
	/** 렌더 타겟을 표시할 화면 머티리얼입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|Screen")
	TObjectPtr<UMaterialInterface> ScreenMaterial;

	/** 렌더 타겟 텍스처를 연결할 머티리얼 파라미터 이름입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|Screen")
	FName RenderTargetParameterName = TEXT("RenderTarget");
	
	/** CCTV 전환 시 출력할 노이즈 속도 파라미터 이름입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|Screen")
	FName NoisePowerParameterName = TEXT("TV.Noise.Power");
	
	/** CCTV 전환 시 출력할 노이즈 강도 파라미터 이름입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|Screen")
	FName NoiseIntensityParameterName = TEXT("TV.Noise.Intensity");
	
	/** 노이즈 출력 시 모니터에 적용할 텍스처입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|Screen")
	UTextureRenderTarget2D* BlackRenderTarget = nullptr;
	
	/** 채널 전환 시 노이즈를 적용할 시간입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|Screen")
	float NoiseEffectDuration = 0.5f;

	/** SCANNING 머티리얼을 유지할 시간입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수|Screen")
	float ScanningDuration = 2.0f;
	
	/** 모니터에 출력할 CCTV 액터 목록입니다. 레벨에 배치된 CCTV 액터를 수동으로 등록해야 합니다.
	 * 
	 * 0 : Basement, 1 : Laundry, 2 : Living,  3 : Toilet
	 */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "변수|CCTV")
	TArray<TObjectPtr<ACCTV>> RegisteredCCTVs;


private:
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> ScreenMaterialInstance;

	UPROPERTY()
	TObjectPtr<UAudioComponent> CCTVNoiseSoundComponent;

	UPROPERTY()
	TObjectPtr<UAudioComponent> ChannelSwitchSoundComponent;

	UPROPERTY()
	TObjectPtr<UAudioComponent> ScanningSoundComponent;

	int32 ActiveCCTVIndex = 0;
	FTimerHandle SwitchTimerHandle;
	FTimerHandle ScanningTimerHandle;
	uint8 bIsScanning : 1 = false;


// Getter, Setter Section
public:
	FORCEINLINE int32 GetActiveCCTVIndex() const { return ActiveCCTVIndex; }
	
};
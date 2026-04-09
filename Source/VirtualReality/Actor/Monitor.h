#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Monitor.generated.h"

class ACCTV;
class UPointLightComponent;
class UStaticMeshComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;

UCLASS()
class VIRTUALREALITY_API AMonitor : public AActor
{
	GENERATED_BODY()

	
// Lifecycle Section	
public:
	AMonitor();
	virtual void BeginPlay() override;
	
	
// Member Function	
public:
	UFUNCTION(BlueprintCallable)
	void SwitchToCCTV(int32 Index);
	
private:
	void SwitchToNextCCTV();
	void SwitchToPrevCCTV();
	void CollectCCTVs();
	void SetActiveCCTV(int32 Index);
	void DeactivateAllCCTVs();

	
// Component Section	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<USceneComponent> MonitorRoot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> MonitorMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> ScreenMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<UPointLightComponent> ScreenLight;
	
	
// Variable Section	
protected:
	/** 렌더 타겟을 표시할 화면 머티리얼입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	TObjectPtr<UMaterialInterface> ScreenMaterial;

	/** 렌더 타겟 텍스처를 연결할 머티리얼 파라미터 이름입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	FName RenderTargetParameterName = TEXT("RenderTarget");

private:
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> ScreenMaterialInstance;

	TArray<TObjectPtr<ACCTV>> RegisteredCCTVs;
	int32 ActiveCCTVIndex = -1;


// Getter, Setter Section
public:
	FORCEINLINE int32 GetActiveCCTVIndex() const { return ActiveCCTVIndex; }
	
};
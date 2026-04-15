#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Entity.generated.h"

UCLASS()
class VIRTUALREALITY_API AEntity : public AActor
{
	GENERATED_BODY()

public:
	AEntity();
	virtual void BeginPlay() override;

private:
	/** BodyMesh와 BodyParts를 병합하여 Body 컴포넌트에 적용합니다. */
	void MergeBodyParts();

public:
	/** 병합 결과를 표시할 스켈레탈 메시 컴포넌트입니다. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수")
	TObjectPtr<USkeletalMeshComponent> Body;

	/** 병합의 베이스가 되는 바디 스켈레탈 메시 에셋입니다. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수")
	TObjectPtr<USkeletalMeshComponent> ClothMesh;

	/** Body에 병합할 파츠 스켈레탈 메시 에셋 목록입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	TArray<TObjectPtr<USkeletalMesh>> BodyParts;
};
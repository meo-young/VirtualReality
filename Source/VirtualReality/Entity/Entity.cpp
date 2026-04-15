#include "Entity.h"
#include "VirtualReality.h"
#include "SkeletalMergingLibrary.h"

AEntity::AEntity()
{
	// Body 컴포넌트를 루트로 설정합니다.
	Body = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	SetRootComponent(Body);
	
	// Body의 옷을 담는 SkeletalMeshComponent를 초기화합니다.
	ClothMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ClothMesh"));
	ClothMesh->SetupAttachment(Body);
}

void AEntity::BeginPlay()
{
	Super::BeginPlay();

	MergeBodyParts();
}

void AEntity::MergeBodyParts()
{
	USkeletalMesh* MergedMesh = NewObject<USkeletalMesh>(GetTransientPackage(), NAME_None, RF_Transient);

	TArray<FSkelMeshMergeSectionMapping> EmptySectionMappings;
	FSkeletalMeshMerge Merger(MergedMesh, BodyParts, EmptySectionMappings, 0);

	if (Merger.DoMerge())
	{
		// 병합 결과는 ClothMesh에 적용, Body(SKM_Body)는 건드리지 않음
		ClothMesh->SetSkeletalMesh(MergedMesh);
		
		// SKM_Body의 포즈를 따라감
		ClothMesh->SetLeaderPoseComponent(Body);
	}
}

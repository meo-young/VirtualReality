#include "Entity.h"
#include "VirtualReality.h"
#include "SkeletalMergingLibrary.h"

AEntity::AEntity()
{
	// Body 컴포넌트를 루트로 설정합니다.
	Body = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	SetRootComponent(Body);
}

void AEntity::BeginPlay()
{
	Super::BeginPlay();

	MergeBodyParts();
}

void AEntity::MergeBodyParts()
{
	// 병합에 필요한 파라미터를 구성합니다.
	FSkeletalMeshMergeParams Params;
	Params.bSkeletonBefore = false;

	// BodyMesh를 첫 번째 파라미터로 추가합니다.
	if (IsValid(BodyMesh))
	{
		Params.MeshesToMerge.Add(BodyMesh);
	}

	// 나머지 파츠들을 추가합니다.
	for (USkeletalMesh* Part : BodyParts)
	{
		if (IsValid(Part))
		{
			Params.MeshesToMerge.Add(Part);
		}
	}
	
	// 메시를 병합하고 Body 컴포넌트에 적용합니다.
	USkeletalMesh* MergedMesh = USkeletalMergingLibrary::MergeMeshes(Params);
	Body->SetSkeletalMesh(MergedMesh);
	
	LOG(TEXT("메시 병합이 완료되었습니다. 파츠 수: %d"), Params.MeshesToMerge.Num());
}

#include "Lever.h"
#include "VirtualReality.h"
#include "Component/VRHapticComponent.h"
#include "Components/BoxComponent.h"
#include "Player/VRHand.h"

ALever::ALever()
{
	PrimaryActorTick.bCanEverTick = true;

	LeverMesh = CreateDefaultSubobject<USkeletalMeshComponent>("LeverMesh");
	LeverMesh->SetupAttachment(Mesh);

	GrabRegion->SetupAttachment(LeverMesh);
	GrabRegion->SetRelativeLocation(FVector(0.0f, -14.0f, 0.0f));
	GrabRegion->SetBoxExtent(FVector(16.0f, 16.0f, 8.0f));

	GrabbableType = EGrabbableType::Lever;
}

void ALever::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentAngle = GetActorRotation().Roll;
}

void ALever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ----- Lock State -----
	if (bIsLocked)
	{
		LockTimer += DeltaTime;
		if (LockTimer >= LockDuration)
		{
			bIsLocked = false;
			bIsReturning = true;
			LOG(TEXT("잠금이 해제되었습니다. MaxAngle로 복귀합니다."));
		}
		return;
	}

	
	
	// ----- Returning State -----
	if (bIsReturning)
	{
		CurrentAngle = FMath::FInterpTo(CurrentAngle, MaxAngle, DeltaTime, ReturnInterpSpeed);
		LeverMesh->SetRelativeRotation(FRotator(0.f, 0.f, CurrentAngle));
		if (FMath::IsNearlyEqual(CurrentAngle, MaxAngle, 1.0f))
		{
			bIsReturning = false;
		}
		return;
	}

	
	
	// ----- Normal State -----
	if (bIsHeld)
	{
		// 레버를 잡고 있는 동안 컨트롤러 위치를 기반으로 각도를 업데이트합니다.
		UpdateLeverAngle(DeltaTime);
	}
	else
	{
		// 레버를 놓은 후 MinAngle에 도달하지 못했다면 MaxAngle로 복귀합니다.
		CurrentAngle = FMath::FInterpTo(CurrentAngle, MaxAngle, DeltaTime, InterpSpeed);
		LeverMesh->SetRelativeRotation(FRotator(0.f, 0.f, CurrentAngle));
	}
}

void ALever::OnGrab(USkeletalMeshComponent* InComponent, const FVector& GrabLocation)
{
	Super::OnGrab(InComponent, GrabLocation);

	// Grab 시점의 컨트롤러 위치와 레버 각도를 캐시합니다.
	CachedHand = Cast<AVRHand>(InComponent->GetOwner());
	if (CachedHand)
	{
		GrabStartControllerZ = CachedHand->GetMotionControllerLocation().Z;
	}
	GrabStartAngle = CurrentAngle;
	bIsHeld = true;
	bReachedMinAngle = false;

	// 손 컴포넌트를 레버의 GrabSocket에 부착합니다.
	InComponent->SetAllBodiesSimulatePhysics(false);
	InComponent->AttachToComponent(LeverMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, GrabSocketName);
}

void ALever::OnRelease(USkeletalMeshComponent* InComponent)
{
	Super::OnRelease(InComponent);

	bIsHeld = false;

	// 햅틱을 즉시 중지합니다.
	if (CachedHand)
	{
		CachedHand->GetHapticComponent()->StopHaptic();
	}

	CachedHand = nullptr;

	// 손 컴포넌트를 레버에서 분리합니다.
	InComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	InComponent->SetAllBodiesSimulatePhysics(true);
}

void ALever::UpdateLeverAngle(float DeltaTime)
{
	if (!CachedHand) return;

	// 컨트롤러가 Grab 시점 대비 얼마나 이동했는지 계산합니다.
	float DeltaZ = CachedHand->GetMotionControllerLocation().Z - GrabStartControllerZ;

	// Delta Z를 각도로 변환합니다. (아래로 내리면 음수 → MinAngle 방향)
	float DeltaAngle = (DeltaZ / MappingRange) * (MaxAngle - MinAngle) * 0.5f;
	float TargetAngle = FMath::Clamp(GrabStartAngle + DeltaAngle, MinAngle, MaxAngle);

	// InterpSpeed가 낮을수록 무거운 느낌을 줍니다.
	CurrentAngle = FMath::FInterpTo(CurrentAngle, TargetAngle, DeltaTime, InterpSpeed);
	LeverMesh->SetRelativeRotation(FRotator(0.0f, 0.f, CurrentAngle));

	// 당기는 정도에 비례하여 연속 햅틱을 재생합니다. (0=MaxAngle, 1=MinAngle)
	float PullRatio = FMath::Clamp((CurrentAngle - MaxAngle) / (MinAngle - MaxAngle), 0.f, 1.f);
	CachedHand->GetHapticComponent()->PlayHaptic(PullHapticFrequency, PullRatio * PullHapticMaxAmplitude);

	// MinAngle에 처음 도달했을 때 잠금을 시작하고 강한 햅틱을 재생합니다.
	if (!bReachedMinAngle && FMath::IsNearlyEqual(CurrentAngle, MinAngle, 10.0f))
	{
		bReachedMinAngle = true;
		bIsLocked = true;
		LockTimer = 0.f;
		CachedHand->GetHapticComponent()->PlayHapticBurst(
			MinAngleHapticFrequency, MinAngleHapticAmplitude, MinAngleHapticDuration);
	}
}
#include "LeverBase.h"
#include "Component/VRHapticComponent.h"
#include "Player/VRHand.h"

ALeverBase::ALeverBase()
{
	// Grab에 사용할 Mesh를 초기화합니다.
	LeverMesh = CreateDefaultSubobject<USkeletalMeshComponent>("LeverMesh");
	LeverMesh->SetupAttachment(Mesh);
}

void ALeverBase::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentAngle = LeverMesh->GetRelativeRotation().Roll;
}

void ALeverBase::Tick(float DeltaTime)
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
		}
		return;
	}



	// ----- Returning State -----
	if (bIsReturning)
	{
		CurrentAngle = FMath::FInterpTo(CurrentAngle, StartAngle, DeltaTime, ReturnInterpSpeed);
		LeverMesh->SetRelativeRotation(FRotator(0.f, 0.f, CurrentAngle));
		if (FMath::IsNearlyEqual(CurrentAngle, StartAngle, 1.0f))
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
		// 레버를 놓은 후 EndAngle 도달하지 못했다면 StartAngle 복귀합니다.
		CurrentAngle = FMath::FInterpTo(CurrentAngle, StartAngle, DeltaTime, ControlInterpSpeed);
		LeverMesh->SetRelativeRotation(FRotator(0.f, 0.f, CurrentAngle));
	}
}

void ALeverBase::DoGrab(USkeletalMeshComponent* InComponent)
{
	Super::DoGrab(InComponent);
	
	// Grab 시점의 컨트롤러 위치와 레버 각도를 캐시합니다.
	if (CachedHand)
	{
		GrabStartControllerY = CachedHand->GetMotionControllerLocation().Y;
	}
	GrabStartAngle = CurrentAngle;

	// 손 컴포넌트를 레버의 GrabSocket에 부착합니다.
	InComponent->SetAllBodiesSimulatePhysics(false);
	InComponent->AttachToComponent(LeverMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, GrabSocketName);
}

void ALeverBase::DoRelease(USkeletalMeshComponent* InComponent)
{
	Super::DoRelease(InComponent);
	
	// 손 컴포넌트를 레버에서 분리합니다.
	InComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	InComponent->SetAllBodiesSimulatePhysics(true);
	bReachedEndAngle = false;
}


void ALeverBase::UpdateLeverAngle(float DeltaTime)
{
	if (!CachedHand) return;

	// 컨트롤러가 Grab 시점 대비 얼마나 Y축으로 이동했는지 계산합니다.
	float DeltaY = CachedHand->GetMotionControllerLocation().Y - GrabStartControllerY;

	// Delta Y를 각도로 변환합니다. (당기면 양수 → EndAngle 방향)
	float DeltaAngle = (DeltaY / MappingRange) * (EndAngle - StartAngle) * 0.5f;
	float TargetAngle = FMath::Clamp(GrabStartAngle + DeltaAngle, StartAngle, EndAngle);

	// InterpSpeed가 낮을수록 무거운 느낌을 줍니다.
	CurrentAngle = FMath::FInterpTo(CurrentAngle, TargetAngle, DeltaTime, ControlInterpSpeed);
	LeverMesh->SetRelativeRotation(FRotator(0.f, 0.f, CurrentAngle));

	// EndAngle에 처음 도달했을 때 잠금을 시작하고 강한 햅틱을 재생합니다.
	if (!bReachedEndAngle && FMath::IsNearlyEqual(CurrentAngle, EndAngle, 10.0f))
	{
		bReachedEndAngle = true;
		bIsLocked = true;
		LockTimer = 0.f;
		CachedHand->GetHapticComponent()->PlayHapticBurst(BurstHapticFrequency, BurstHapticAmplitude, BurstHapticDuration);
		CachedHand->ReleaseObject();
	}
}

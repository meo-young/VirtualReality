#include "PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "InputActionValue.h"
#include "MotionControllerComponent.h"
#include "VirtualReality.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// 액터가 컨트롤러 Yaw를 따라 회전하도록 활성화합니다.
	bUseControllerRotationYaw = true;

	// VR 트래킹 원점 (발 기준, 캡슐에 부착)
	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VROrigin"));
	VROrigin->SetupAttachment(GetRootComponent());

	// HMD 카메라 (VRRoot에 부착)
	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(VROrigin);

	// 왼손 컨트롤러 (VRRoot에 부착)
	LeftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftController"));
	LeftController->SetupAttachment(VROrigin);
	LeftController->SetTrackingMotionSource(FName("Left"));

	// 오른손 컨트롤러 (VRRoot에 부착)
	RightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightController"));
	RightController->SetupAttachment(VROrigin);
	RightController->SetTrackingMotionSource(FName("Right"));
	
	// 캐릭터 SkeletalMesh (VRCamera에 부착)
	GetMesh()->SetupAttachment(VRCamera);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EIC)
	{
		LOG(TEXT("EnhancedInputComponent를 찾을 수 없습니다."));
		return;
	}

	// 이동 액션 바인딩 (Triggered: 매 프레임 입력 유지 중 호출)
	EIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerCharacter::DoMove);

	// 회전 액션 바인딩
	EIC->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &APlayerCharacter::DoTurn);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InitVRSetting();
	
	// 현재 컨트롤러 Yaw로 보간 초기값을 설정합니다.
	TargetYaw   = GetControlRotation().Yaw;
	SmoothedYaw = TargetYaw;
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	LerpOffsetToHMD();
	ScaleCapsuleToHMD();
	LerpControlRotation(DeltaSeconds);
}

void APlayerCharacter::InitVRSetting()
{
	// HMD가 연결되어 있으면 발 기준 트래킹 원점으로 설정합니다.
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::LocalFloor);
		LOG(TEXT("HMD 감지 — 트래킹 원점을 Floor로 설정했습니다."));
	}
	else
	{
		LOG(TEXT("HMD가 감지되지 않았습니다. 일반 카메라 모드로 실행됩니다."));
	}
}

void APlayerCharacter::LerpOffsetToHMD()
{
	FVector CameraLocation = VRCamera->GetComponentLocation();
	FVector CapsuleLocation = GetCapsuleComponent()->GetComponentLocation();

	CameraLocation.Z = 0.0f;
	CapsuleLocation.Z = 0.0f;

	GetCapsuleComponent()->AddWorldOffset(CameraLocation - CapsuleLocation, false);
	VROrigin->AddWorldOffset(CapsuleLocation - CameraLocation);
}

void APlayerCharacter::ScaleCapsuleToHMD()
{
	FVector CameraLocation = VRCamera->GetRelativeLocation();
	FVector VRLocation = VROrigin->GetRelativeLocation();
	VRLocation.Z = -CameraLocation.Z/2 - 2.15f;

	VROrigin->SetRelativeLocation(VRLocation);
	GetCapsuleComponent()->SetCapsuleHalfHeight(CameraLocation.Z/2, true);
}

void APlayerCharacter::LerpControlRotation(const float InDeltaTime)
{
	// SmoothedYaw를 TargetYaw 방향으로 부드럽게 보간하여 컨트롤러에 적용합니다.
	// bUseControllerRotationYaw = true 이므로 액터가 자동으로 따라옵니다.
	SmoothedYaw = FMath::FInterpTo(SmoothedYaw, TargetYaw, InDeltaTime, TurnLagSpeed);
	AController* C = GetController();
	if (C)
	{
		C->SetControlRotation(FRotator(0.f, SmoothedYaw, 0.f));
	}
}

void APlayerCharacter::DoMove(const FInputActionValue& InValue)
{
	const FVector2D MoveInput = InValue.Get<FVector2D>();
	if (MoveInput.IsNearlyZero()) return;
	
	// 몸체(액터)의 전방/우방 벡터 기준으로 이동 방향을 계산합니다.
	AddMovementInput(GetActorForwardVector(), MoveInput.Y);
	AddMovementInput(GetActorRightVector(),   MoveInput.X);
}

void APlayerCharacter::DoTurn(const FInputActionValue& InValue)
{
	const float TurnInput = InValue.Get<float>();
	if (FMath::IsNearlyZero(TurnInput)) return;

	// 목표 Yaw에 입력값을 누적합니다. 실제 회전은 Tick에서 보간하여 적용합니다.
	TargetYaw += TurnInput * TurnSpeed * GetWorld()->GetDeltaSeconds();
}

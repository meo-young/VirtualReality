#include "Flash.h"

#include "VirtualReality.h"
#include "Actor/EventZone.h"
#include "Components/SpotLightComponent.h"
#include "Define/Define.h"
#include "Player/VRHand.h"
#include "EngineUtils.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"

AFlash::AFlash()
{
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Flash(TEXT("/Game/_VirtualReality/Mesh/Flash/SM_Flash"));
	if (SM_Flash.Succeeded())
	{
		Mesh->SetStaticMesh(SM_Flash.Object);
	}
	
	FlashLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashLight"));
	FlashLight->SetupAttachment(Mesh);
	
	GrabbableType = EGrabbableType::Flash;
}

void AFlash::BeginPlay()
{
	Super::BeginPlay();

	SetEnableLightVisibility(false);

	// 후레쉬를 왼손에 영구 부착합니다. (그랩/릴리즈 불가, 검지 트리거로 켜고/끄기만 가능)
	TryAttachToHand();
}

void AFlash::TryAttachToHand()
{
	// 레벨에서 왼손을 찾아 소켓에 영구 부착합니다.
	for (TActorIterator<AVRHand> It(GetWorld()); It; ++It)
	{
		AVRHand* Hand = *It;
		if (Hand->GetHandType() != EControllerHand::Left) continue;

		USkeletalMeshComponent* TargetHandMesh = Hand->GetHandMesh();
		if (!TargetHandMesh) break;

		// 물리를 끄고 왼손 소켓에 부착합니다.
		Mesh->SetSimulatePhysics(false);
		Mesh->AttachToComponent(TargetHandMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftGrabSocketName);

		// 검지 트리거로 켜고/끄기 토글하도록 손에 등록하고, GrabbableType(Flash)에 맞는 손 애니메이션을 적용합니다.
		Hand->SetAttachedFlashlight(this, GetGrabbableType());

		// 그랩/릴리즈 대상에서 제외하여 손에서 절대 떨어지지 않게 합니다.
		bIsLocked = true;
		GrabRegion->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		GetWorldTimerManager().ClearTimer(AttachRetryTimerHandle);
		LOG(TEXT("후레쉬를 왼손에 부착했습니다."));
		return;
	}

	// 아직 손이 스폰되지 않았다면 잠시 후 재시도합니다.
	GetWorldTimerManager().SetTimer(AttachRetryTimerHandle, this, &AFlash::TryAttachToHand, 0.1f, false);
}

void AFlash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	IsIrradiateEventZone();
}

void AFlash::Interact()
{
	SetEnableLightVisibility(!FlashLight->IsVisible());
}

void AFlash::SetLightEnabled(bool bEnabled)
{
	SetEnableLightVisibility(bEnabled);
}

void AFlash::SetEnableLightVisibility(bool InEnableLightVisibility)
{
	FlashLight->SetVisibility(InEnableLightVisibility);
	SetActorTickEnabled(InEnableLightVisibility);

	// 라이트를 끄면 더 이상 구역을 비추지 않으므로 감지 상태를 초기화합니다.
	if (!InEnableLightVisibility)
	{
		bIsIrradiateEventZone = false;
	}
}

bool AFlash::IsIrradiateEventZone()
{
	FHitResult HitResult;
	const FVector StartLocation = FlashLight->GetComponentLocation(); // 메시 내부 대신 라이트 위치
	const FVector EndLocation = StartLocation + 2000.f * FlashLight->GetForwardVector();

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // 자기 자신 제외

	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_FLASH, Params);

	FColor Color;
	
	if (HitResult.GetActor())
	{
		bIsIrradiateEventZone = true;
		Color = FColor::Red;

		if (AEventZone* EventZone = Cast<AEventZone>(HitResult.GetActor()))
		{
			EventZone->OnFlashHit();
		}
	}
	else
	{
		bIsIrradiateEventZone = false;
		Color = FColor::Silver;
	}
	
	//DrawDebugLine(GetWorld(), StartLocation, EndLocation, Color);

	return HitResult.GetActor() != nullptr;

}

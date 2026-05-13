#include "Flash.h"

#include "VirtualReality.h"
#include "Actor/EventZone.h"
#include "Components/SpotLightComponent.h"
#include "Define/Define.h"
#include "Player/VRHand.h"

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
}

void AFlash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	IsIrradiateEventZone();
}

void AFlash::DoGrab(USkeletalMeshComponent* InComponent)
{
	Super::DoGrab(InComponent);
	
	// HandType에 따라 SocketName을 다르게 적용합니다.
	FName GrabSocketName;
	CachedHand->GetHandType() == EControllerHand::Right ? GrabSocketName = RightGrabSocketName : GrabSocketName = LeftGrabSocketName;
	
	// 메시의 물리엔진을 비활성화 하고 Socket에 부착합니다.
	Mesh->SetSimulatePhysics(false);
	bIsHeld = Mesh->AttachToComponent(InComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, GrabSocketName);
	if (bIsHeld)
	{
		GrabbedBySkeletalMesh = InComponent;
	}
}

void AFlash::DoRelease(USkeletalMeshComponent* InComponent)
{
	Super::DoRelease(InComponent);
	
	if (bIsHeld)
	{
		if (GrabbedBySkeletalMesh == InComponent)
		{
			Mesh->SetSimulatePhysics(true);
		}
	}
	
	SetEnableLightVisibility(false);
}

void AFlash::Interact()
{
	SetEnableLightVisibility(!FlashLight->IsVisible());
}

void AFlash::SetEnableLightVisibility(bool InEnableLightVisibility)
{
	FlashLight->SetVisibility(InEnableLightVisibility);
	SetActorTickEnabled(InEnableLightVisibility);
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

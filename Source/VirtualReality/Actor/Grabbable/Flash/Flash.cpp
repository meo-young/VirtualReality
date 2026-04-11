#include "Flash.h"
#include "Components/SpotLightComponent.h"
#include "Player/VRHand.h"

AFlash::AFlash()
{
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
	
	FlashLight->SetVisibility(false);
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
	
	FlashLight->SetVisibility(false);
}

void AFlash::Interact()
{
	FlashLight->SetVisibility(!FlashLight->IsVisible());
}

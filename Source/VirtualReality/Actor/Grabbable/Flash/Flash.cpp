#include "Flash.h"
#include "Components/SpotLightComponent.h"

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

void AFlash::OnGrab(USkeletalMeshComponent* InComponent, const FVector& GrabLocation)
{
	Super::OnGrab(InComponent, GrabLocation);
	
	Mesh->SetSimulatePhysics(false);
	bIsHeld = Mesh->AttachToComponent(InComponent, FAttachmentTransformRules::KeepWorldTransform, GrabSocketName);
	if (bIsHeld)
	{
		GrabbedBySkeletalMesh = InComponent;
	}
}

void AFlash::OnRelease(USkeletalMeshComponent* InComponent)
{
	Super::OnRelease(InComponent);
	
	if (bIsHeld)
	{
		if (GrabbedBySkeletalMesh == InComponent)
		{
			Mesh->SetSimulatePhysics(true);
			bIsHeld = false;
		}
	}
}

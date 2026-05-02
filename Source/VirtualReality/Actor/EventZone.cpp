#include "EventZone.h"
#include "Components/ShapeComponent.h"
#include "Define/Define.h"

AEventZone::AEventZone()
{
	GetCollisionComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCollisionComponent()->SetCollisionResponseToChannel(ECC_FLASH, ECR_Block);
}

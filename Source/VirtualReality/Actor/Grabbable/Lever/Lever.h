#pragma once

#include "CoreMinimal.h"
#include "Actor/Grabbable/VRGrabbableActor.h"
#include "Lever.generated.h"

UCLASS()
class VIRTUALREALITY_API ALever : public AVRGrabbableActor
{
	GENERATED_BODY()
	
public:
	ALever();
	
public:
	virtual void OnGrab(USkeletalMeshComponent* InComponent, const FVector& GrabLocation) override;
	virtual void OnRelease(USkeletalMeshComponent* InComponent) override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|컴포넌트")
	TObjectPtr<USkeletalMeshComponent> LeverMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "변수|소켓")
	FName GrabSocketName = FName(TEXT("GrabSocket"));

};

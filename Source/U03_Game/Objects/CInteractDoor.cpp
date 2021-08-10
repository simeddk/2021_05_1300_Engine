#include "CInteractDoor.h"
#include "Global.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

ACInteractDoor::ACInteractDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent(this, &Scene, "Scene");
	CHelpers::CreateComponent(this, &Box, "Box", Scene);
	CHelpers::CreateComponent(this, &DoorFrame, "DoorFrame", Scene);
	CHelpers::CreateComponent(this, &Door, "Door", DoorFrame);

	UStaticMesh* doorFrame;
	CHelpers::GetAsset<UStaticMesh>(&doorFrame, "StaticMesh'/Game/DoorMesh/Props/SM_DoorFrame.SM_DoorFrame'");
	DoorFrame->SetStaticMesh(doorFrame);

	UStaticMesh* door;
	CHelpers::GetAsset<UStaticMesh>(&door, "StaticMesh'/Game/DoorMesh/Props/SM_Door.SM_Door'");
	Door->SetStaticMesh(door);

	Box->InitBoxExtent(FVector(150, 50, 100));
	Box->SetRelativeLocation(FVector(0, 0, 100));
	Box->SetCollisionProfileName("Trigger");

	Door->SetRelativeLocation(FVector(FVector(0, 45, 0)));
}

void ACInteractDoor::BeginPlay()
{
	Super::BeginPlay();
	
	Box->SetHiddenInGame(bHiddenInGame);
}

void ACInteractDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACInteractDoor::Interact(const FVector& InForward)
{
}


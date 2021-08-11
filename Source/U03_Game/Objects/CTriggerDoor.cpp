#include "CTriggerDoor.h"
#include "Global.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Characters/CPlayer.h"

ACTriggerDoor::ACTriggerDoor()
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

	Door->SetRelativeLocation(FVector(0, 45, 0));
}

void ACTriggerDoor::BeginPlay()
{
	Super::BeginPlay();

	Box->SetHiddenInGame(bHiddenEdLevel);

	Box->OnComponentBeginOverlap.AddDynamic(this, &ACTriggerDoor::OnComponentBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACTriggerDoor::OnComponentEndOverlap);

	OnTriggerDoorOpen.AddUFunction(this, "Open");
	OnTriggerDoorClose.AddUFunction(this, "Close");
}

void ACTriggerDoor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNull(OtherActor);
	CheckNull(OtherComp);
	CheckTrue(OtherActor == this);
	CheckNull(Cast<ACPlayer>(OtherActor));

	if (OnTriggerDoorOpen.IsBound())
		OnTriggerDoorOpen.Broadcast(Cast<ACPlayer>(OtherActor));
}

void ACTriggerDoor::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckNull(OtherActor);
	CheckNull(OtherComp);
	CheckTrue(OtherActor == this);
	CheckNull(Cast<ACPlayer>(OtherActor));

	if (OnTriggerDoorClose.IsBound())
		OnTriggerDoorClose.Broadcast();
}

void ACTriggerDoor::Open(ACPlayer* InPlayer)
{
	FVector doorForward = GetActorForwardVector();
	FVector playerForward = InPlayer->GetActorForwardVector();

	float direction = FMath::Sign(doorForward | playerForward);
	Degree = direction * MaxDegree;

	bOpen = true;
}

void ACTriggerDoor::Close()
{
	bOpen = false;
}

void ACTriggerDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator rotator = Door->GetRelativeRotation();

	if (bOpen)
		Door->SetRelativeRotation(FMath::Lerp(rotator, FRotator(0, Degree, 0), Speed));
	else
		Door->SetRelativeRotation(FMath::Lerp(rotator, FRotator(0, 0, 0), Speed));

}


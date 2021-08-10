#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CInteractDoor.generated.h"

UCLASS()
class U03_GAME_API ACInteractDoor : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Door")
		float Speed = 80.0f;

	UPROPERTY(EditAnywhere, Category = "Door")
		float MaxDegree = 90.0f;

	UPROPERTY(EditAnywhere, Category = "Door")
		bool bHiddenInGame;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* Box;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* DoorFrame;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Door;
	
public:	
	ACInteractDoor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void Interact(const FVector& InForward);

};

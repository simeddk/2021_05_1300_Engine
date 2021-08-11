#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CTriggerDoor.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FTriggerDoorOpen, class ACPlayer*);
DECLARE_MULTICAST_DELEGATE(FTriggerDoorClose);

UCLASS()
class U03_GAME_API ACTriggerDoor : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Door")
		float Speed = 0.02f;

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
	FTriggerDoorOpen OnTriggerDoorOpen;
	FTriggerDoorClose OnTriggerDoorClose;

public:	
	ACTriggerDoor();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void Open(class ACPlayer* InPlayer);

	UFUNCTION()
		void Close();


public:	
	virtual void Tick(float DeltaTime) override;


private:
	bool bOpen = false;
	float Degree;
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "CCameraActor.generated.h"

UCLASS()
class U03_GAME_API ACCameraActor : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;
	
public:	
	ACCameraActor();

public:
	UFUNCTION(BlueprintCallable)
		void StartTimeline();

private:
	UFUNCTION()
		void OnProcess(float Output);

	UFUNCTION(BlueprintCallable)
		void OnEndProcess();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


private:
	FTimeline Timeline;
	class ACCameraSpline* Spline;
};

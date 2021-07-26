#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/TimelineComponent.h"
#include "CAim.generated.h"

UCLASS()
class U03_GAME_API UCAim : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		class UCurveFloat* Curve;

public:
	FORCEINLINE bool IsAvaliable() { return SpringArm != nullptr && Camera != nullptr; }
	FORCEINLINE bool IsInZoom() { return bInZoom; }

public:
	UCAim();

	void BeginPlay(class ACharacter* InCharacter);
	void Tick(float DeltaTime);

	void On();
	void Off();

private:
	UFUNCTION()
		void Zooming(float Output);

private:
	class ACharacter* OwnerCharacter;
	class UCStateComponent* State;
	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;
	bool bInZoom;

	FTimeline Timeline;
	FOnTimelineFloat TimelineFloat;

	class ACHUD* Hud;
};

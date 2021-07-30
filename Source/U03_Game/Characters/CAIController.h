#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

UCLASS()
class U03_GAME_API ACAIController : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		float BehaviorRange = 150.0f;

	UPROPERTY(EditAnywhere)
		bool bDrawDebug = true;

	UPROPERTY(EditAnywhere)
		float DebugHeight = 50.0f;
		
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UAIPerceptionComponent* Perception;

	UPROPERTY(VisibleDefaultsOnly)
		class UCBehaviorComponent* Behavior;

public:
	FORCEINLINE float GetBehaviorRange() { return BehaviorRange; }

public:
	ACAIController();
	virtual void Tick(float DeltaTime) override;

public:
	float GetSightRadius();

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	UFUNCTION()
		void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

private:
	class ACEnemy_AI* OwnerEnemy;
	
	class UAISenseConfig_Sight* Sight;
};

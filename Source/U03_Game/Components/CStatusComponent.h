#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"

UENUM(BlueprintType)
enum class ECharacterSpeed : uint8
{
	Walk, Run, Sprint, Max
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U03_GAME_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Health")
		float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Speed")
		float Speed[(int32)ECharacterSpeed::Max] = { 200, 400, 1200 };

public:
	FORCEINLINE float GetMaxHealth(){ return MaxHealth; }
	FORCEINLINE float GetHealth(){ return Health; }

public:
	FORCEINLINE float GetWalkSpeed() { return Speed[(int32)ECharacterSpeed::Walk]; }
	FORCEINLINE float GetRunSpeed() { return Speed[(int32)ECharacterSpeed::Run]; }
	FORCEINLINE float GetSprintSpeed() { return Speed[(int32)ECharacterSpeed::Sprint]; }

	FORCEINLINE bool CanMove() { return bCanMove; }

	void SetMove();
	void SetStop();

	void SetSpeed(ECharacterSpeed InSpeed);

	void AddHealth(float InAmount);
	void SubHealth(float InAmount);

public:	
	UCStatusComponent();

protected:
	virtual void BeginPlay() override;

private:
	float Health;
	bool bCanMove = true;
		
};

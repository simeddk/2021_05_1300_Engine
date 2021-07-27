#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CBehaviorComponent.generated.h"

UENUM(BlueprintType)
enum class EBehaviorType : uint8
{
	Wait, Approach, Action, Patrol, Hitted, Avoid
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBehaviorTypeChanged, EBehaviorType, InPrevType, EBehaviorType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U03_GAME_API UCBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		FName BehaviorKey = "Behavior";

	UPROPERTY(EditAnywhere)
		FName PlayerKey = "Player";

	UPROPERTY(EditAnywhere)
		FName WarpLocationKey = "WarpLocation";

public:
	FORCEINLINE void SetBlackBoard(class UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard;}
	
public:
	class ACPlayer* GetTargetPlayer();
	FVector GetWarpLocation();

private:
	void ChangeType(EBehaviorType InType);
	EBehaviorType GetType();

public:
	UFUNCTION(BlueprintCallable) bool IsWaitMode();
	UFUNCTION(BlueprintCallable) bool IsApproachMode();
	UFUNCTION(BlueprintCallable) bool IsActionMode();
	UFUNCTION(BlueprintCallable) bool IsPatrolMode();
	UFUNCTION(BlueprintCallable) bool IsHittedMode();
	UFUNCTION(BlueprintCallable) bool IsAvoidMode();

	void SetWaitMode();
	void SetApproachMode();
	void SetActionMode();
	void SetPatrolMode();
	void SetHittedMode();
	void SetAvoidMode();

public:	
	UCBehaviorComponent();

protected:
	virtual void BeginPlay() override;


public:
	UPROPERTY(BlueprintAssignable)
		FBehaviorTypeChanged OnBehaviorTypeChanged;

private:
	class UBlackboardComponent* Blackboard;
		
};

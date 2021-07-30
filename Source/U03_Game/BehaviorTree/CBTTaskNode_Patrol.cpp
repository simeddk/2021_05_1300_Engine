#include "CBTTaskNode_Patrol.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CPatrolComponent.h"

UCBTTaskNode_Patrol::UCBTTaskNode_Patrol()
{
	NodeName = "Patrol";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());

	ACEnemy_AI* aiPawn = Cast<ACEnemy_AI>(controller->GetPawn());
	UCPatrolComponent* patrol = CHelpers::GetComponent<UCPatrolComponent>(aiPawn);

	FVector location;
	float accpeptance;
	if (patrol->GetMoveTo(location, accpeptance) == false)
		return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress;

	
}

void UCBTTaskNode_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	
	ACEnemy_AI* aiPawn = Cast<ACEnemy_AI>(controller->GetPawn());
	UCPatrolComponent* patrol = CHelpers::GetComponent<UCPatrolComponent>(aiPawn);

	FVector location;
	float accpeptance;
	patrol->GetMoveTo(location, accpeptance);

	EPathFollowingRequestResult::Type type = controller->MoveToLocation(location, accpeptance, false);

	if (type == EPathFollowingRequestResult::Failed)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (type == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		patrol->UpdateNextIndex();
	}
	
}
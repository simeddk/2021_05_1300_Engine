#include "CBTTaskNode_Action.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"
#include "Components/CActionComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CBehaviorComponent.h"

UCBTTaskNode_Action::UCBTTaskNode_Action()
{
	NodeName = "Action";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Action::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* aiPawn = Cast<ACEnemy_AI>(controller->GetPawn());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(aiPawn);

	//CheckNullResult(behavior->GetTargetPlayer(), EBTNodeResult::Failed);
	//UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);
	//controller->SetFocus(behavior->GetTargetPlayer());
	controller->StopMovement();

	RunningTime = 0.0f;
	action->DoAction();
	
	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Action::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* aiPawn = Cast<ACEnemy_AI>(controller->GetPawn());

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(aiPawn);

	RunningTime += DeltaSeconds;

	if (state->IsIdleMode() && RunningTime > Delay)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		//controller->ClearFocus(EAIFocusPriority::Gameplay);
	}
}

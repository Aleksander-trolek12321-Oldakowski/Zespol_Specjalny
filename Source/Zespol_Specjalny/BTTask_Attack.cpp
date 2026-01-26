#include "BTTask_Attack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy_MeleeGreatSword.h"
#include "AIEnemyController.h"

UBTTask_Attack::UBTTask_Attack()
{
    NodeName = "Melee Attack";
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon) return EBTNodeResult::Failed;

    APawn* Pawn = AICon->GetPawn();
    if (!Pawn) return EBTNodeResult::Failed;

    AEnemy_MeleeGreatSword* Melee = Cast<AEnemy_MeleeGreatSword>(Pawn);
    if (!Melee) return EBTNodeResult::Failed;

    Melee->StartAttack();

    return EBTNodeResult::Succeeded;
}

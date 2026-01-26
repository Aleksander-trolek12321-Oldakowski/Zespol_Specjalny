#include "AIEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "EnemyBase.h"
#include "GameFramework/Character.h"

AAIEnemyController::AAIEnemyController()
{
    BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
    BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
    BehaviorTreeAsset = nullptr;
}

void AAIEnemyController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    AEnemyBase* Enemy = Cast<AEnemyBase>(InPawn);
    if (Enemy)
    {
        if (BehaviorTreeAsset)
        {
            if (BlackboardComp)
            {
                UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BlackboardComp);
            }
            BehaviorComp->StartTree(*BehaviorTreeAsset);
        }
    }
}

void AAIEnemyController::OnUnPossess()
{
    Super::OnUnPossess();
    if (BehaviorComp)
    {
        BehaviorComp->StopTree();
    }
}

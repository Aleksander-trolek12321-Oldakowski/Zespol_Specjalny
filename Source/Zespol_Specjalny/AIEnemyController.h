#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIEnemyController.generated.h"

UCLASS()
class ZESPOL_SPECJALNY_API AAIEnemyController : public AAIController
{
    GENERATED_BODY()

public:
    AAIEnemyController();

    virtual void OnPossess(APawn* InPawn) override;
    virtual void OnUnPossess() override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UBehaviorTree* BehaviorTreeAsset;

    UPROPERTY()
    UBlackboardComponent* BlackboardComp;

    UPROPERTY()
    UBehaviorTreeComponent* BehaviorComp;
};

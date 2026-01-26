#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckPlayer.generated.h"

UCLASS()
class ZESPOL_SPECJALNY_API UBTService_CheckPlayer : public UBTService
{
    GENERATED_BODY()

public:
    UBTService_CheckPlayer();

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, Category="AI")
    float CheckInterval;
};

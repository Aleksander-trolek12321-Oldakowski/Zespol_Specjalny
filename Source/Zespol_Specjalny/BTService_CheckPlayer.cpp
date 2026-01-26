#include "BTService_CheckPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIEnemyController.h"
#include "EnemyBase.h"
#include "MainCharacter.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

UBTService_CheckPlayer::UBTService_CheckPlayer()
{
    bNotifyTick = true;
    CheckInterval = 0.3f;
}

void UBTService_CheckPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!BB || !AICon) return;

    APawn* AIPawn = AICon->GetPawn();
    if (!AIPawn) return;

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(AIPawn->GetWorld(), 0);
    if (!PlayerPawn)
    {
        BB->SetValueAsObject(FName(TEXT("TargetActor")), nullptr);
        BB->SetValueAsBool(FName(TEXT("HasLOS")), false);
        BB->SetValueAsBool(FName(TEXT("InAttackRange")), false);
        return;
    }

    FVector Start = AIPawn->GetActorLocation() + FVector(0.f, 0.f, 50.f);
    FVector End = PlayerPawn->GetActorLocation() + FVector(0.f, 0.f, 50.f);

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(AIPawn);

    bool bHit = AIPawn->GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

    AActor* HitActor = Hit.GetActor();
    bool bHasLOS = (!bHit) || (HitActor == PlayerPawn);

    BB->SetValueAsObject(FName(TEXT("TargetActor")), PlayerPawn);
    BB->SetValueAsBool(FName(TEXT("HasLOS")), bHasLOS);

    float Dist = FVector::Dist(AIPawn->GetActorLocation(), PlayerPawn->GetActorLocation());
    float AttackRange = 200.f;
    AEnemyBase* Enemy = Cast<AEnemyBase>(AIPawn);
    if (Enemy)
    {
        AttackRange = Enemy->AttackRange;
    }
    BB->SetValueAsBool(FName(TEXT("InAttackRange")), Dist <= AttackRange);
}

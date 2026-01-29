#include "EnemySpawner.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnemyBase.h"
#include "AttributesComponent.h"

AEnemySpawner::AEnemySpawner()
{
    PrimaryActorTick.bCanEverTick = false;
    SpawnRadius = 200.f;
    BaseCountPerWave = 3;
    BaseSpawnInterval = 0.5f;
}

void AEnemySpawner::BeginPlay()
{
    Super::BeginPlay();
}

void AEnemySpawner::StartSpawningWave(int32 Count, float Interval, float HealthMultiplier)
{
    if (!EnemyClass) return;

    StopSpawning();

    RemainingToSpawn = Count;
    CurrentHealthMultiplier = HealthMultiplier;

    float FirstDelay = 0.f;
    GetWorldTimerManager().SetTimer(TimerHandle_Spawn, this, &AEnemySpawner::SpawnTick, Interval, true, FirstDelay);
    BaseSpawnInterval = Interval;
}

void AEnemySpawner::StopSpawning()
{
    if (GetWorld()) GetWorldTimerManager().ClearTimer(TimerHandle_Spawn);
    RemainingToSpawn = 0;
}

void AEnemySpawner::SpawnTick()
{
    if (RemainingToSpawn <= 0)
    {
        StopSpawning();
        return;
    }

    SpawnOne();
    RemainingToSpawn--;
}

AEnemyBase* AEnemySpawner::SpawnOne()
{
    if (!EnemyClass) return nullptr;
    UWorld* W = GetWorld();
    if (!W) return nullptr;

    FVector SpawnLoc = GetActorLocation();
    if (SpawnRadius > 0.f)
    {
        FVector Rand = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.f, SpawnRadius);
        SpawnLoc += Rand;
    }

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    AEnemyBase* E = W->SpawnActor<AEnemyBase>(EnemyClass, SpawnLoc, GetActorRotation(), Params);
    if (E)
    {
        if (E->Attributes)
        {
            E->Attributes->MaxHP *= CurrentHealthMultiplier;
            E->Attributes->CurrentHP = E->Attributes->MaxHP;
        }

        E->OnEnemyDeath.AddDynamic(this, &AEnemySpawner::OnSpawnedEnemyDeath);
    }
    return E;
}

void AEnemySpawner::OnSpawnedEnemyDeath(AEnemyBase* DeadEnemy)
{
}

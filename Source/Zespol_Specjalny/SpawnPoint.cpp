#include "SpawnPoint.h"
#include "EnemyBase.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "AttributesComponent.h"

ASpawnPoint::ASpawnPoint()
{
    PrimaryActorTick.bCanEverTick = false;
    CurrentSpawnInterval = SpawnInterval;
    SetActorTickEnabled(false);
}

void ASpawnPoint::BeginPlay()
{
    Super::BeginPlay();
    CurrentSpawnInterval = SpawnInterval;
}

void ASpawnPoint::AdjustForDifficulty(float HealthMultiplier, float SpawnIntervalMultiplier, int32 AdditionalPerWave)
{
    CurrentHealthMultiplier = HealthMultiplier;
    CurrentSpawnInterval = FMath::Max(0.05f, SpawnInterval * SpawnIntervalMultiplier);
    CurrentAdditionalPerWave = AdditionalPerWave;
}

void ASpawnPoint::SpawnEnemies(int32 CountOverride)
{
    if (!bEnabled || !GetWorld()) return;
    if (!EnemyClass) return;

    const int32 Count = (CountOverride > 0) ? CountOverride : (SpawnCountPerWave + CurrentAdditionalPerWave);

    for (int32 i = 0; i < Count; ++i)
    {
        FVector RandomOffset(FMath::FRandRange(-30.f, 30.f), FMath::FRandRange(-30.f, 30.f), 0.f);
        FVector SpawnLocation = GetActorLocation() + SpawnOffset + RandomOffset;
        FRotator SpawnRot = GetActorRotation();

        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        AEnemyBase* NewEnemy = GetWorld()->SpawnActor<AEnemyBase>(EnemyClass, SpawnLocation, SpawnRot, Params);
        if (!NewEnemy) continue;

        if (NewEnemy->Attributes)
        {
            NewEnemy->Attributes->MaxHP *= CurrentHealthMultiplier;
            NewEnemy->Attributes->CurrentHP = NewEnemy->Attributes->MaxHP;
        }

        NewEnemy->SpawnDefaultController();

    }
}

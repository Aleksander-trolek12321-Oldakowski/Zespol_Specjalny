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

TSubclassOf<AEnemyBase> ASpawnPoint::ChooseEnemyClassToSpawn() const
{
    if (SpawnableEnemies.Num() > 0)
    {
        int32 Index = FMath::RandRange(0, SpawnableEnemies.Num() - 1);
        TSubclassOf<AEnemyBase> Chosen = SpawnableEnemies[Index];
        if (Chosen)
        {
            return Chosen;
        }
    }

    return EnemyClass;
}

void ASpawnPoint::SpawnEnemies(int32 CountOverride)
{
    if (!bEnabled || !GetWorld()) return;

    const int32 Count = (CountOverride > 0) ? CountOverride : (SpawnCountPerWave + CurrentAdditionalPerWave);

    for (int32 i = 0; i < Count; ++i)
    {
        FVector RandomOffset(FMath::FRandRange(-30.f, 30.f), FMath::FRandRange(-30.f, 30.f), 0.f);
        FVector SpawnLocation = GetActorLocation() + SpawnOffset + RandomOffset;
        FRotator SpawnRot = GetActorRotation();

        TSubclassOf<AEnemyBase> ChosenClass = ChooseEnemyClassToSpawn();
        if (!ChosenClass)
        {
            UE_LOG(LogTemp, Warning, TEXT("%s: No Enemy class available to spawn (SpawnableEnemies empty and EnemyClass null)."), *GetName());
            continue;
        }

        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        AEnemyBase* NewEnemy = GetWorld()->SpawnActor<AEnemyBase>(ChosenClass, SpawnLocation, SpawnRot, Params);
        if (!NewEnemy)
        {
            UE_LOG(LogTemp, Warning, TEXT("%s: Failed to spawn enemy of class %s at %s"), *GetName(), *ChosenClass->GetName(), *SpawnLocation.ToString());
            continue;
        }

        UE_LOG(LogTemp, Log, TEXT("%s: Spawned enemy %s (class %s) at %s"), *GetName(), *NewEnemy->GetName(), *NewEnemy->GetClass()->GetName(), *SpawnLocation.ToString());

        if (NewEnemy->Attributes)
        {
            NewEnemy->Attributes->MaxHP *= CurrentHealthMultiplier;
            NewEnemy->Attributes->CurrentHP = NewEnemy->Attributes->MaxHP;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("%s: Spawned enemy %s has no Attributes component."), *GetName(), *NewEnemy->GetName());
        }

        NewEnemy->SpawnDefaultController();
    }
}

#include "WaveManager.h"
#include "EnemySpawner.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "EngineUtils.h"

AWaveManager::AWaveManager()
{
    PrimaryActorTick.bCanEverTick = false;
    BaseEnemiesPerWave = 10;
    BaseTimeBetweenWaves = 15.f;
    HealthMultiplierPerWave = 1.15f;
    SpawnIntervalMultiplierPerWave = 0.95f;
    CurrentWave = 0;
    bRunning = false;
}

void AWaveManager::BeginPlay()
{
    Super::BeginPlay();

    if (Spawners.Num() == 0)
    {
        for (TActorIterator<AEnemySpawner> It(GetWorld()); It; ++It)
        {
            Spawners.Add(*It);
        }
    }
}

void AWaveManager::StartWaves()
{
    if (bRunning) return;
    bRunning = true;
    CurrentWave = 0;
    CurrentHealthMultiplier = 1.0f;
    CurrentSpawnInterval = 1.0f;

    StartNextWave();
}

void AWaveManager::StopWaves()
{
    bRunning = false;
    GetWorldTimerManager().ClearTimer(TimerHandle_NextWave);
}

void AWaveManager::StartNextWave()
{
    if (!bRunning) return;

    CurrentWave++;

    CurrentHealthMultiplier = FMath::Pow(HealthMultiplierPerWave, CurrentWave - 1);

    float spawnInterval = 1.0f * FMath::Pow(SpawnIntervalMultiplierPerWave, CurrentWave - 1);

    int32 totalEnemies = BaseEnemiesPerWave + (CurrentWave - 1) * 2;

    int32 perSpawner = FMath::Max(1, totalEnemies / FMath::Max(1, Spawners.Num()));

    for (AEnemySpawner* S : Spawners)
    {
        if (!S) continue;
        S->StartSpawningWave(perSpawner, spawnInterval, CurrentHealthMultiplier);
    }

    float timeToNext = FMath::Max(3.f, BaseTimeBetweenWaves * FMath::Pow(0.98f, CurrentWave - 1));

    GetWorldTimerManager().SetTimer(TimerHandle_NextWave, this, &AWaveManager::StartNextWave, timeToNext, false);
}
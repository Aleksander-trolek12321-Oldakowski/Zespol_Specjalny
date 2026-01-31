#include "WaveManager.h"
#include "SpawnPoint.h"
#include "EngineUtils.h"
#include "TimerManager.h"
#include "Engine/World.h"

AWaveManager::AWaveManager()
{
    PrimaryActorTick.bCanEverTick = false;
    WaveIndex = 0;
    bRunning = false;
}

void AWaveManager::BeginPlay()
{
    Super::BeginPlay();

    for (TActorIterator<ASpawnPoint> It(GetWorld()); It; ++It)
    {
        ASpawnPoint* SP = *It;
        if (SP)
        {
            Spawners.Add(SP);
        }
    }

    StartWaves();
}

void AWaveManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    StopWaves();
    Super::EndPlay(EndPlayReason);
}

void AWaveManager::RegisterSpawner(ASpawnPoint* Spawner)
{
    if (!Spawner) return;
    if (!Spawners.Contains(Spawner))
    {
        Spawners.Add(Spawner);
    }
}

void AWaveManager::StartWaves()
{
    if (bRunning) return;
    bRunning = true;
    WaveIndex = 0;

    GetWorld()->GetTimerManager().SetTimer(TimerHandle_NextWave, this, &AWaveManager::TickWave, TimeBeforeStart, false);
}

void AWaveManager::StopWaves()
{
    bRunning = false;
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle_NextWave);
}

void AWaveManager::TickWave()
{
    if (!bRunning) return;

    WaveIndex++;
    OnWaveStarted.Broadcast(WaveIndex);

    float HealthMultiplier = FMath::Pow(HealthMultiplierPerWave, WaveIndex - 1);
    float SpawnIntervalMultiplier = FMath::Pow(SpawnIntervalMultiplierPerWave, WaveIndex - 1);

    int32 AdditionalPerWave = FMath::FloorToInt(static_cast<float>(AdditionalEnemiesPerWave) * static_cast<float>(WaveIndex - 1));

    for (ASpawnPoint* SP : Spawners)
    {
        if (!SP) continue;
        SP->AdjustForDifficulty(HealthMultiplier, SpawnIntervalMultiplier, AdditionalPerWave);
        SP->SpawnEnemies();
    }

    float IntervalNext = TimeBetweenWaves * SpawnIntervalMultiplier;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_NextWave, this, &AWaveManager::TickWave, IntervalNext, false);
}

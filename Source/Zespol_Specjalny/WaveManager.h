#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveManager.generated.h"

class ASpawnPoint;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveStartedSignature, int32, WaveIndex);

UCLASS(Blueprintable)
class ZESPOL_SPECJALNY_API AWaveManager : public AActor
{
    GENERATED_BODY()

public:
    AWaveManager();

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UFUNCTION(BlueprintCallable, Category="Waves")
    void StartWaves();

    UFUNCTION(BlueprintCallable, Category="Waves")
    void StopWaves();

    UFUNCTION(BlueprintCallable, Category="Waves")
    void RegisterSpawner(ASpawnPoint* Spawner);

    UPROPERTY(BlueprintAssignable, Category="Waves")
    FOnWaveStartedSignature OnWaveStarted;

protected:
    void TickWave();
    void ScheduleNextWave();

    TArray<ASpawnPoint*> Spawners;

    int32 WaveIndex;
    FTimerHandle TimerHandle_NextWave;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Waves")
    float TimeBetweenWaves = 6.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Waves")
    float TimeBeforeStart = 3.f;

    // skalowanie trudności na falę
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Waves")
    float HealthMultiplierPerWave = 1.05f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Waves")
    float SpawnIntervalMultiplierPerWave = 0.98f; 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Waves")
    int32 AdditionalEnemiesPerWave = 0;

    bool bRunning;
};

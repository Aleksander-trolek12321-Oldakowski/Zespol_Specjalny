#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveManager.generated.h"

class AEnemySpawner;

UCLASS()
class ZESPOL_SPECJALNY_API AWaveManager : public AActor
{
    GENERATED_BODY()

public:
    AWaveManager();

    UFUNCTION(BlueprintCallable, Category="Waves")
    void StartWaves();

    UFUNCTION(BlueprintCallable, Category="Waves")
    void StopWaves();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Waves")
    TArray<AEnemySpawner*> Spawners;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Waves")
    int32 BaseEnemiesPerWave;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Waves")
    float BaseTimeBetweenWaves;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Waves")
    float HealthMultiplierPerWave;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Waves")
    float SpawnIntervalMultiplierPerWave;

protected:
    virtual void BeginPlay() override;

private:
    void StartNextWave();
    FTimerHandle TimerHandle_NextWave;
    int32 CurrentWave;
    float CurrentHealthMultiplier;
    float CurrentSpawnInterval;
    bool bRunning;
};

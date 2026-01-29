#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class AEnemyBase;
class AWaveManager;

UCLASS()
class ZESPOL_SPECJALNY_API AEnemySpawner : public AActor
{
    GENERATED_BODY()

public:
    AEnemySpawner();

    UFUNCTION(BlueprintCallable, Category="Spawner")
    void StartSpawningWave(int32 Count, float Interval, float HealthMultiplier);

    UFUNCTION(BlueprintCallable, Category="Spawner")
    void StopSpawning();

    UFUNCTION(BlueprintCallable, Category="Spawner")
    AEnemyBase* SpawnOne();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawner")
    TSubclassOf<AEnemyBase> EnemyClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawner")
    float SpawnRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawner")
    int32 BaseCountPerWave;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawner")
    float BaseSpawnInterval;

protected:
    virtual void BeginPlay() override;

private:
    void SpawnTick();
    int32 RemainingToSpawn;
    float CurrentHealthMultiplier;
    FTimerHandle TimerHandle_Spawn;

    UFUNCTION()
    void OnSpawnedEnemyDeath(AEnemyBase* DeadEnemy);
};

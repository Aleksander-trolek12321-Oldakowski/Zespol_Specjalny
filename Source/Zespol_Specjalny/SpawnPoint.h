#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnPoint.generated.h"

class AEnemyBase;

UCLASS(Blueprintable)
class ZESPOL_SPECJALNY_API ASpawnPoint : public AActor
{
    GENERATED_BODY()

public:
    ASpawnPoint();

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category="Spawn")
    void SpawnEnemies(int32 CountOverride = -1);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
    TSubclassOf<AEnemyBase> EnemyClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn", meta=(ClampMin="0.01"))
    float SpawnInterval = 5.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn", meta=(ClampMin="1"))
    int32 SpawnCountPerWave = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
    FVector SpawnOffset = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
    bool bEnabled = true;

    UFUNCTION(BlueprintCallable, Category="Spawn")
    void AdjustForDifficulty(float HealthMultiplier, float SpawnIntervalMultiplier, int32 AdditionalPerWave);

protected:
    float CurrentSpawnInterval;
    float CurrentHealthMultiplier = 1.f;
    int32 CurrentAdditionalPerWave = 0;
};

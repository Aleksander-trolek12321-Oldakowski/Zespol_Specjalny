#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "Enemy_MeleeGreatSword.generated.h"

class UAnimMontage;

UCLASS()
class ZESPOL_SPECJALNY_API AEnemy_MeleeGreatSword : public AEnemyBase
{
    GENERATED_BODY()

public:
    AEnemy_MeleeGreatSword();

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void StartAttack();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void ExecuteAttackHit();

	UFUNCTION(BlueprintCallable, Category="Combat")
	void StartHitWindow();

protected:
    void ResetAttack();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    UAnimMontage* AttackMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float AttackHitDelay;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float AttackCooldown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    FName WeaponSocketName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float HitSphereRadius;

    bool bCanAttack;
    bool bHasDealtHit;

    FTimerHandle TimerHandle_ResetAttack;
};

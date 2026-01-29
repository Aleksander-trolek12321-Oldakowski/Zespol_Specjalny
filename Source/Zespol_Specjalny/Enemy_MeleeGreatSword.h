#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "Components/BoxComponent.h"
#include "Enemy_MeleeGreatSword.generated.h"

class UAnimMontage;

UCLASS()
class ZESPOL_SPECJALNY_API AEnemy_MeleeGreatSword : public AEnemyBase
{
    GENERATED_BODY()

public:
    AEnemy_MeleeGreatSword();

    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void StartAttack();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void ExecuteAttackHit();

	UFUNCTION(BlueprintCallable, Category="Combat")
	void StartHitWindow();

    UFUNCTION()
    void OnSwordOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    virtual void Die() override;

    UFUNCTION(BlueprintCallable, Category="Combat")
    void EnableSwordHitbox();

    UFUNCTION(BlueprintCallable, Category="Combat")
    void DisableSwordHitbox();

    UFUNCTION(BlueprintCallable, Category="Combat")
    void ClearAlreadyHitActors();

protected:
    void ResetAttack();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat")
    UBoxComponent* SwordHitBox;

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug")
    bool bDrawSwordDebugBox;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
    FVector SwordHalfExtent = FVector(80.f, 3.f, 3.f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
    FRotator SwordRotationOffset = FRotator::ZeroRotator;

    bool bCanAttack;
    bool bHasDealtHit;

    FTimerHandle TimerHandle_ResetAttack;

    TSet<TWeakObjectPtr<AActor>> AlreadyHitActors;
};

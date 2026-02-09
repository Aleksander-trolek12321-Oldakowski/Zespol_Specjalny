#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h" 
#include "EnemyBase.generated.h"

class UAttributesComponent;
class UStaticMeshComponent;
class UWidgetComponent;
class UBehaviorTree;
class APickupBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDeathSignature, AEnemyBase*, Enemy);

UCLASS(Blueprintable)
class ZESPOL_SPECJALNY_API AEnemyBase : public ACharacter
{
    GENERATED_BODY()

public:
    AEnemyBase();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
    UAttributesComponent* Attributes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float AttackRange;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* VisualMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UWidgetComponent* HPWidgetComponent;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
    void ReceiveDamage(float Amount);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
    void Attack();

    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetHPPercent() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    void UpdateHPWidget();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    virtual void Die();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
    void OnDeathBP();
    virtual void OnDeathBP_Implementation();

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnEnemyDeathSignature OnEnemyDeath;

    UPROPERTY(BlueprintReadWrite, Category="AI")
    AActor* CurrentTarget = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
    float RotationSpeed = 10.f;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UBehaviorTree* EnemyBehaviorTree;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gameplay")
    int32 ScoreValue = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drops")
    TArray<TSubclassOf<AActor>> DropTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drops")
    float DropChance = 0.25f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drops")
    float DropSpawnZOffset = 40.f;

    UFUNCTION(BlueprintCallable, Category = "Drops")
    void TryDropLoot();

protected:
    bool bIsDead;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    UAnimMontage* DeathMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float LifeSpanAfterDeath = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    UParticleSystem* DeathVFX;
};

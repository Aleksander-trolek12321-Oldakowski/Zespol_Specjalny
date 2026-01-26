#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

class UAttributesComponent;
class UStaticMeshComponent;
class UWidgetComponent;

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

protected:
    bool bIsDead;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    UAnimMontage* DeathMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float LifeSpanAfterDeath = 5.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    UParticleSystem* DeathVFX;
};

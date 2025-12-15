#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

class UAttributesComponent;
class UStaticMeshComponent;
class UWidgetComponent;

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

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
    void OnDeath();

protected:
    bool bIsDead;
};

// AttributesComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributesComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZESPOL_SPECJALNY_API UAttributesComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAttributesComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    float MaxHP;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
    float CurrentHP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    float Stamina;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    float Damage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    float Speed;

    UFUNCTION(BlueprintCallable, Category = "Attributes")
    float ApplyDamage(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Attributes")
    void Heal(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Attributes")
    float GetHPPercent() const;

    UFUNCTION(BlueprintCallable, Category = "Attributes")
    void ResetAttributes();

protected:
    virtual void BeginPlay() override;
};

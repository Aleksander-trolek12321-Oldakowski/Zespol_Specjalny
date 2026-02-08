#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TopDownPlayerState.generated.h"

UCLASS()
class ZESPOL_SPECJALNY_API ATopDownPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    ATopDownPlayerState();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Pickups")
    float BonusMaxHealth = 0.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Pickups")
    int32 BonusAmmoCapacity = 0;

    UFUNCTION(BlueprintCallable)
    void AddMaxHealthBonus(float Amount);

    UFUNCTION(BlueprintCallable)
    void AddAmmoCapacityBonus(int32 Amount);
};
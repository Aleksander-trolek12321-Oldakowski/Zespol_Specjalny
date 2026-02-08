#include "TopDownPlayerState.h"

ATopDownPlayerState::ATopDownPlayerState()
{
    BonusMaxHealth = 0.f;
    BonusAmmoCapacity = 0;
}

void ATopDownPlayerState::AddMaxHealthBonus(float Amount)
{
    BonusMaxHealth += Amount;
}

void ATopDownPlayerState::AddAmmoCapacityBonus(int32 Amount)
{
    BonusAmmoCapacity += Amount;
}
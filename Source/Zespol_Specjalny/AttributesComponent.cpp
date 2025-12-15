#include "AttributesComponent.h"

UAttributesComponent::UAttributesComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    MaxHP = 100.f;
    CurrentHP = MaxHP;
    Stamina = 100.f;
    Damage = 10.f;
    Speed = 300.f;
}

void UAttributesComponent::BeginPlay()
{
    Super::BeginPlay();

    CurrentHP = FMath::Clamp(CurrentHP, 0.f, MaxHP);
}

float UAttributesComponent::ApplyDamage(float Amount)
{
    CurrentHP -= Amount;
    CurrentHP = FMath::Clamp(CurrentHP, 0.f, MaxHP);
    return CurrentHP;
}

void UAttributesComponent::Heal(float Amount)
{
    CurrentHP += Amount;
    CurrentHP = FMath::Clamp(CurrentHP, 0.f, MaxHP);
}

float UAttributesComponent::GetHPPercent() const
{
    if (MaxHP <= 0.f) return 0.f;
    return CurrentHP / MaxHP;
}

void UAttributesComponent::ResetAttributes()
{
    CurrentHP = MaxHP;
}

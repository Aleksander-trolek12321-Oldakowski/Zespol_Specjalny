#include "EnemyBase.h"
#include "AttributesComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyBase::AEnemyBase()
{
    PrimaryActorTick.bCanEverTick = true;

    Attributes = CreateDefaultSubobject<UAttributesComponent>(TEXT("Attributes"));

    AttackRange = 200.f;
    bIsDead = false;

    VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
    VisualMesh->SetupAttachment(GetCapsuleComponent());
    VisualMesh->SetRelativeLocation(FVector(0.f, 0.f, -90.f));

    HPWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWidget"));
    HPWidgetComponent->SetupAttachment(RootComponent);
    HPWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 110.f));
    HPWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    HPWidgetComponent->SetDrawSize(FVector2D(120.f, 20.f));

	Attributes->MaxHP = 100.f;
	Attributes->CurrentHP = 100.f;

    GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AEnemyBase::BeginPlay()
{
    Super::BeginPlay();

    if (Attributes)
    {
        Attributes->CurrentHP = FMath::Clamp(Attributes->CurrentHP, 0.0f, Attributes->MaxHP);

        if (GetCharacterMovement())
        {
            GetCharacterMovement()->MaxWalkSpeed = Attributes->Speed;
        }
    }

    UpdateHPWidget();
}

void AEnemyBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

float AEnemyBase::GetHPPercent() const
{
    if (Attributes)
    {
        return Attributes->GetHPPercent();
    }
    return 0.0f;
}

void AEnemyBase::UpdateHPWidget()
{
    if (!HPWidgetComponent) return;
    UUserWidget* W = HPWidgetComponent->GetUserWidgetObject();
    if (!W) return;
}

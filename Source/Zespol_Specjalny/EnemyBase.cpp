#include "EnemyBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AttributesComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

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

void AEnemyBase::Die()
{
    if (bIsDead) return;
    bIsDead = true;

    OnEnemyDeath.Broadcast(this);

    AController* C = GetController();
    if (C)
    {
        if (AAIController* AICon = Cast<AAIController>(C))
        {
            if (UBrainComponent* Brain = AICon->GetBrainComponent())
            {
                Brain->StopLogic(TEXT("Dead"));
            }
            AICon->UnPossess();
        }
        else
        {
            C->UnPossess();
        }
    }

    if (GetCharacterMovement())
    {
        GetCharacterMovement()->DisableMovement();
        GetCharacterMovement()->StopMovementImmediately();
    }
    if (GetCapsuleComponent())
    {
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
    if (GetMesh())
    {
        GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    if (DeathMontage && GetMesh())
    {
        if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
        {
            AnimInst->Montage_Play(DeathMontage);
        }
    }

    if (DeathVFX)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathVFX, GetActorLocation(), GetActorRotation());
    }

    OnDeathBP();

    SetLifeSpan(LifeSpanAfterDeath);
}

void AEnemyBase::OnDeathBP_Implementation()
{
}

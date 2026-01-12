#include "Enemy_MeleeGreatSword.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"
#include "CollisionShape.h"
#include "Engine/Engine.h"
#include "WorldCollision.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AttributesComponent.h"

AEnemy_MeleeGreatSword::AEnemy_MeleeGreatSword()
{
    PrimaryActorTick.bCanEverTick = true;

    AttackMontage = nullptr;
    AttackRange = 150.f;
    AttackHitDelay = 0.45f;
    AttackCooldown = 1.2f;

    WeaponSocketName = TEXT("WeaponSocket");
    HitSphereRadius = 18.f;

    bCanAttack = true;
    bHasDealtHit = false;
}

void AEnemy_MeleeGreatSword::BeginPlay()
{
    Super::BeginPlay();
}

void AEnemy_MeleeGreatSword::StartHitWindow()
{
    bHasDealtHit = false;
}

void AEnemy_MeleeGreatSword::StartAttack()
{
    if (!bCanAttack) return;

    bCanAttack = false;
    bHasDealtHit = false;

    if (AttackMontage && GetMesh())
    {
        UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
        if (AnimInst)
        {
            AnimInst->Montage_Play(AttackMontage);
        }
    }

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(TimerHandle_ResetAttack, this, &AEnemy_MeleeGreatSword::ResetAttack, AttackCooldown, false);
    }
}

void AEnemy_MeleeGreatSword::ExecuteAttackHit()
{
    if (bHasDealtHit) return;
    bHasDealtHit = true;

    if (!GetWorld() || !GetMesh()) return;

    FVector SocketLocation = GetMesh()->GetSocketLocation(WeaponSocketName);

    #if WITH_EDITOR
    DrawDebugSphere(GetWorld(), SocketLocation, HitSphereRadius, 12, FColor::Red, false, 1.5f);
    #endif

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

    UClass* ActorClassFilter = AMainCharacter::StaticClass();

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);

    TArray<AActor*> OutActors;

    bool bAny = UKismetSystemLibrary::SphereOverlapActors(
        GetWorld(),
        SocketLocation,
        HitSphereRadius,
        ObjectTypes,
        ActorClassFilter,
        ActorsToIgnore,
        OutActors
    );

    if (!bAny || OutActors.Num() == 0) return;

    for (AActor* HitActor : OutActors)
    {
        if (!HitActor) continue;
        if (HitActor == this) continue;

        AMainCharacter* Player = Cast<AMainCharacter>(HitActor);
        if (Player)
        {
            float DamageToDeal = 10.f;
            if (Attributes)
            {
                DamageToDeal = Attributes->Damage;
            }

            UGameplayStatics::ApplyDamage(Player, DamageToDeal, GetController(), this, nullptr);
        }
    }
}

void AEnemy_MeleeGreatSword::ResetAttack()
{
    bCanAttack = true;
    bHasDealtHit = false;
}

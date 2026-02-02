#include "Enemy_MeleeGreatSword.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/BoxComponent.h"
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
    HitSphereRadius = 24.f;

    bDrawSwordDebugBox = true;
    bCanAttack = true;
    bHasDealtHit = false;

    SwordHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordHitBox"));

    SwordHitBox->SetupAttachment(GetMesh());
    SwordHitBox->SetBoxExtent(SwordHalfExtent);
    SwordHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SwordHitBox->SetGenerateOverlapEvents(false);
    SwordHitBox->SetCollisionObjectType(ECC_WorldDynamic);
    SwordHitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    SwordHitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    SwordHitBox->SetHiddenInGame(false);
}

void AEnemy_MeleeGreatSword::BeginPlay()
{
    Super::BeginPlay();

    if (!SwordHitBox)
    {
        UE_LOG(LogTemp, Error, TEXT("%s: SwordHitBox is null in BeginPlay!"), *GetName());
        return;
    }

    USkeletalMeshComponent* MeshComp = GetMesh();
    if (!MeshComp)
    {
        MeshComp = FindComponentByClass<USkeletalMeshComponent>();
    }

    if (!MeshComp)
    {
        UE_LOG(LogTemp, Error, TEXT("%s: No SkeletalMeshComponent found in BeginPlay!"), *GetName());
        return;
    }

    if (MeshComp->DoesSocketExist(WeaponSocketName))
    {
        SwordHitBox->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
        SwordHitBox->SetRelativeLocation(FVector::ZeroVector);
        SwordHitBox->SetRelativeRotation(SwordRotationOffset);
        SwordHitBox->SetBoxExtent(SwordHalfExtent);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("%s: Socket '%s' does NOT exist - attaching to mesh root."), *GetName(), *WeaponSocketName.ToString());
        SwordHitBox->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    }

    SwordHitBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_MeleeGreatSword::OnSwordOverlap);

    const FTransform T = SwordHitBox->GetComponentTransform();
    UE_LOG(LogTemp, Log, TEXT("%s: SwordHitBox attached -> WorldLoc=%s WorldRot=%s Extent=%s"),
        *GetName(),
        *T.GetLocation().ToString(),
        *T.GetRotation().Rotator().ToCompactString(),
        *SwordHitBox->GetUnscaledBoxExtent().ToString()
    );

    if (bDrawSwordDebugBox)
    {
        DrawDebugBox(GetWorld(), T.GetLocation(), SwordHitBox->GetUnscaledBoxExtent(), T.GetRotation(), FColor::Red, false, 5.f, 0, 2.f);
    }

    if (HPWidgetComponent)
    {
        HPWidgetComponent->SetHiddenInGame(false);
        HPWidgetComponent->SetVisibility(true);

        HPWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
        HPWidgetComponent->SetDrawSize(FVector2D(120.f, 20.f));

        UUserWidget* W = HPWidgetComponent->GetUserWidgetObject();
        if (!W)
        {
            UE_LOG(LogTemp, Warning, TEXT("%s: HPWidgetComponent istnieje, ale nie ma UserWidget (brak przypisanej Widget Class lub nie utworzono instancji). Sprawdź BP przeciwnika."), *GetName());
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("%s: HPWidgetComponent instance found: %s — wywołuję UpdateHPWidget()"), *GetName(), *W->GetName());
            UpdateHPWidget();
        }

        HPWidgetComponent->SetOwnerNoSee(false);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("%s: Brak HPWidgetComponent (powinien być zadeklarowany w EnemyBase)."), *GetName());
    }

    AlreadyHitActors.Empty();
}

void AEnemy_MeleeGreatSword::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateHPWidget();
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
    FQuat SocketQuat = GetMesh()->GetSocketQuaternion(WeaponSocketName);

    const float HalfLength = 80.f;
    const float HalfWidth  = 3.f;
    const float HalfThickness = 3.f;
    const FVector BoxHalfExtent(HalfLength, HalfWidth, HalfThickness);

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    FCollisionShape BoxShape = FCollisionShape::MakeBox(BoxHalfExtent);

    TArray<FHitResult> HitResults;

    const FVector Start = SocketLocation;
    const FVector End = SocketLocation;

    bool bHit = GetWorld()->SweepMultiByChannel(
        HitResults,
        Start,
        End,
        SocketQuat,
        ECC_Pawn,
        BoxShape,
        Params
    );

    if (!bHit || HitResults.Num() == 0) return;

    for (const FHitResult& Hit : HitResults)
    {
        AActor* HitActor = Hit.GetActor();
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

void AEnemy_MeleeGreatSword::OnSwordOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this) return;
    if (AlreadyHitActors.Contains(OtherActor)) return;

    AMainCharacter* Player = Cast<AMainCharacter>(OtherActor);
    if (Player)
    {
        AlreadyHitActors.Add(OtherActor);
        float DamageToDeal = Attributes ? Attributes->Damage : 10.f;
        UGameplayStatics::ApplyDamage(Player, DamageToDeal, GetController(), this, nullptr);
    }
}

void AEnemy_MeleeGreatSword::ResetAttack()
{
    bCanAttack = true;
    bHasDealtHit = false;
}

void AEnemy_MeleeGreatSword::EnableSwordHitbox()
{
    if (SwordHitBox)
    {
        AlreadyHitActors.Empty();
        SwordHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("%s: EnableSwordHitbox called but SwordHitBox == nullptr"), *GetName());
    }
}

void AEnemy_MeleeGreatSword::DisableSwordHitbox()
{
    if (SwordHitBox)
    {
        SwordHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("%s: DisableSwordHitbox called but SwordHitBox == nullptr"), *GetName());
    }
}

void AEnemy_MeleeGreatSword::ClearAlreadyHitActors()
{
    AlreadyHitActors.Empty();
}

void AEnemy_MeleeGreatSword::Die()
{
    Super::Die();
}

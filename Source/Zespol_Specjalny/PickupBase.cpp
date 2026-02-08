#include "PickupBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"

APickupBase::APickupBase()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    CollisionComp->InitSphereRadius(40.f);
    CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComp->SetCollisionObjectType(ECC_WorldDynamic);
    CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
    CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    CollisionComp->SetGenerateOverlapEvents(true);
    RootComponent = CollisionComp;

    Visual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual"));
    Visual->SetupAttachment(RootComponent);
    Visual->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnOverlapBegin);
}

void APickupBase::BeginPlay()
{
    Super::BeginPlay();
}

void APickupBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
    if (!OtherActor) return;
    APawn* Pawn = Cast<APawn>(OtherActor);
    if (!Pawn) return;

    bool bHandled = OnPicked(Pawn);

    if (bHandled)
    {
        Destroy();
    }
}

bool APickupBase::OnPicked_Implementation(APawn* InstigatorPawn)
{
    AMainCharacter* Player = Cast<AMainCharacter>(InstigatorPawn);
    if (Player)
    {
        Player->Heal(HealAmount);
        return true;
    }
    return false;
}
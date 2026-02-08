#include "PickupBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"

APickupBase::APickupBase()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    RootComponent = MeshComp;
    MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);

    Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
    Trigger->SetupAttachment(RootComponent);
    Trigger->SetSphereRadius(50.f);
    Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void APickupBase::BeginPlay()
{
    Super::BeginPlay();
    if (Trigger)
    {
        Trigger->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnOverlapBegin);
    }
}

void APickupBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                const FHitResult& SweepResult)
{
    if (!OtherActor) return;
    APawn* Pawn = Cast<APawn>(OtherActor);
    if (!Pawn) return;

    if (OnPicked(Pawn))
    {
        if (bDestroyOnPickup)
        {
            Destroy();
        }
    }
}

bool APickupBase::OnPicked_Implementation(APawn* InstigatorPawn)
{
    return false;
}
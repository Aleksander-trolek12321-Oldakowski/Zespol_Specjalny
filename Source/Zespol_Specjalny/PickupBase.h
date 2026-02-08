#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupBase.generated.h"

UCLASS()
class ZESPOL_SPECJALNY_API APickupBase : public AActor
{
    GENERATED_BODY()

public:
    APickupBase();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup")
    UStaticMeshComponent* MeshComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pickup")
    class USphereComponent* Trigger;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup")
    bool bDestroyOnPickup = true;

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                const FHitResult& SweepResult);

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Pickup")
	bool OnPicked(APawn* InstigatorPawn);
    virtual bool OnPicked_Implementation(APawn* InstigatorPawn);
};
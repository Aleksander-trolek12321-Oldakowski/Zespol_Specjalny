#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupBase.generated.h"

class USphereComponent;

UCLASS()
class ZESPOL_SPECJALNY_API APickupBase : public AActor
{
    GENERATED_BODY()

public:
    APickupBase();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pickup")
    USphereComponent* CollisionComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pickup")
    UStaticMeshComponent* Visual;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup")
    float HealAmount = 25.f;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Pickup")
    bool OnPicked(APawn* InstigatorPawn);

    virtual bool OnPicked_Implementation(APawn* InstigatorPawn);

protected:
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                        const FHitResult& SweepResult);
};
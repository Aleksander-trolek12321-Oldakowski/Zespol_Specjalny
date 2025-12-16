#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HeroCharacter.generated.h"

UCLASS()
class ENDLESSSHOOTER_API AHeroCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Konstruktor - tu ustawiamy wartoœci domyœlne
    AHeroCharacter();

protected:
    // Funkcja wywo³ywana, gdy gra startuje
    virtual void BeginPlay() override;

public:
    // Funkcja wywo³ywana w ka¿dej klatce gry (np. 60 razy na sekundê)
    virtual void Tick(float DeltaTime) override;

    // Funkcja do wi¹zania klawiszy (wyjaœnimy to przy ruchu)
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // --- KOMPONENTY (Co postaæ posiada fizycznie) ---

    // SpringArm to "wysiêgnik" trzymaj¹cy kamerê (¿eby nie przenika³a przez œciany)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class USpringArmComponent* CameraBoom;

    // Sama kamera
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class UCameraComponent* FollowCamera;

    // --- STATYSTYKI (Zgodnie z wymaganiami) ---

    // Makro EditAnywhere pozwala edytowaæ te wartoœci w edytorze Unreal
    // BlueprintReadWrite pozwala Blueprintom (np. paskowi zdrowia) widzieæ tê zmienn¹

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxHealth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    float CurrentHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxMana; // Nasz "zasób"

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    float CurrentMana;
};
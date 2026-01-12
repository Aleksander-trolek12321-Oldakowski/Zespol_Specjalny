#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class ZESPOL_SPECJALNY_API AMainCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Konstruktor - tu ustawiamy warto�ci domy�lne
    AMainCharacter();

protected:
    // Funkcja wywo�ywana, gdy gra startuje
    virtual void BeginPlay() override;

public:
    // Funkcja wywo�ywana w ka�dej klatce gry (np. 60 razy na sekund�)
    virtual void Tick(float DeltaTime) override;

    // Funkcja do wi�zania klawiszy (wyja�nimy to przy ruchu)
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // --- KOMPONENTY (Co posta� posiada fizycznie) ---

    // SpringArm to "wysi�gnik" trzymaj�cy kamer� (�eby nie przenika�a przez �ciany)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class USpringArmComponent* CameraBoom;

    // Sama kamera
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class UCameraComponent* FollowCamera;

    // --- STATYSTYKI (Zgodnie z wymaganiami) ---

    // Makro EditAnywhere pozwala edytowa� te warto�ci w edytorze Unreal
    // BlueprintReadWrite pozwala Blueprintom (np. paskowi zdrowia) widzie� t� zmienn�

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxHealth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    float CurrentHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxMana; // Nasz "zas�b"

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    float CurrentMana;
};
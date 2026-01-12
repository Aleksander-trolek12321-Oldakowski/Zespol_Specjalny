#include "MainCharacter.h"
// Musimy do��czy� nag��wki komponent�w, kt�rych u�ywamy
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Konstruktor
AMainCharacter::AMainCharacter()
{
    // W��czamy Tick, je�li potrzebny (cz�sto mo�na wy��czy� dla optymalizacji, ale na razie zostawmy)
    PrimaryActorTick.bCanEverTick = true;

    // 1. Ustawienia postaci
    // Nie chcemy, aby posta� obraca�a si� razem z kamer� (w grach top-down kamera patrzy z g�ry, posta� obraca si� myszk�/klawiszami)
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Ustawiamy, by posta� obraca�a si� w kierunku ruchu
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

    // 2. Tworzymy SpringArm (Wysi�gnik)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent); // Przyczepiamy do korzenia postaci (kapsu�y)

    // Ustawiamy wysi�gnik w tryb Top-Down
    CameraBoom->SetUsingAbsoluteRotation(true); // Nie chcemy, by rami� obraca�o si�, gdy posta� si� przewraca
    CameraBoom->TargetArmLength = 800.f; // Odleg�o�� kamery od gracza (wysoko��)
    CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f)); // Pochylenie kamery w d� o 60 stopni
    CameraBoom->bDoCollisionTest = false; // �eby kamera nie przybli�a�a si� "dziwnie", gdy wejdziemy pod drzewo

    // 3. Tworzymy Kamer�
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Przyczepiamy do ko�ca wysi�gnika
    FollowCamera->bUsePawnControlRotation = false; // Kamera nie pod��a za obrotem kontrolera

    // 4. Inicjalizacja statystyk
    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;
    MaxMana = 50.0f;
    CurrentMana = MaxMana;
}

void AMainCharacter::BeginPlay()
{
    Super::BeginPlay();
    // Na starcie upewniamy si�, �e zdrowie jest pe�ne
    CurrentHealth = MaxHealth;
}

void AMainCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    // Tu b�dziemy dodawa� sterowanie w kolejnym kroku!
}

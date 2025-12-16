#include "HeroCharacter.h"
// Musimy do³¹czyæ nag³ówki komponentów, których u¿ywamy
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Konstruktor
AHeroCharacter::AHeroCharacter()
{
    // W³¹czamy Tick, jeœli potrzebny (czêsto mo¿na wy³¹czyæ dla optymalizacji, ale na razie zostawmy)
    PrimaryActorTick.bCanEverTick = true;

    // 1. Ustawienia postaci
    // Nie chcemy, aby postaæ obraca³a siê razem z kamer¹ (w grach top-down kamera patrzy z góry, postaæ obraca siê myszk¹/klawiszami)
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Ustawiamy, by postaæ obraca³a siê w kierunku ruchu
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

    // 2. Tworzymy SpringArm (Wysiêgnik)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent); // Przyczepiamy do korzenia postaci (kapsu³y)

    // Ustawiamy wysiêgnik w tryb Top-Down
    CameraBoom->SetUsingAbsoluteRotation(true); // Nie chcemy, by ramiê obraca³o siê, gdy postaæ siê przewraca
    CameraBoom->TargetArmLength = 800.f; // Odleg³oœæ kamery od gracza (wysokoœæ)
    CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f)); // Pochylenie kamery w dó³ o 60 stopni
    CameraBoom->bDoCollisionTest = false; // ¯eby kamera nie przybli¿a³a siê "dziwnie", gdy wejdziemy pod drzewo

    // 3. Tworzymy Kamerê
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Przyczepiamy do koñca wysiêgnika
    FollowCamera->bUsePawnControlRotation = false; // Kamera nie pod¹¿a za obrotem kontrolera

    // 4. Inicjalizacja statystyk
    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;
    MaxMana = 50.0f;
    CurrentMana = MaxMana;
}

void AHeroCharacter::BeginPlay()
{
    Super::BeginPlay();
    // Na starcie upewniamy siê, ¿e zdrowie jest pe³ne
    CurrentHealth = MaxHealth;
}

void AHeroCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    // Tu bêdziemy dodawaæ sterowanie w kolejnym kroku!
}

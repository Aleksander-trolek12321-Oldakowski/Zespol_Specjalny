#include "MainCharacter.h"
#include "GMB_TopDown.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

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

	// 5. Tworzenie komponentu Widget (pasek zdrowia nad g��ow�)
    // Create the Component
    HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));

    // Attach it to the Root (so it follows the player)
    HealthWidgetComp->SetupAttachment(RootComponent);

    // Set where it sits relative to the player 
    // (Z=120 usually puts it just above a standard human head)
    HealthWidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));

    // Set the mode to "Screen" so it always faces the camera (classic RPG style)
    // If you want it flat in the world, use EWidgetSpace::World
    HealthWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);

    // Set the size of the draw area
    HealthWidgetComp->SetDrawSize(FVector2D(100.0f, 15.0f));

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

float AMainCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    if (ActualDamage <= 0.f) return 0.f;

    CurrentHealth = FMath::Clamp(CurrentHealth - ActualDamage, 0.f, MaxHealth);

    UE_LOG(LogTemp, Log, TEXT("%s otrzymal %f obrazen. HP: %f/%f"), *GetName(), ActualDamage, CurrentHealth, MaxHealth);


    if (CurrentHealth <= 0.f && !bIsDead)
    {
        Die();
    }

    return ActualDamage;
}

void AMainCharacter::Die()
{
    if (bIsDead) return;
    bIsDead = true;

    APlayerController* PC = Cast<APlayerController>(GetController());
    if (PC)
    {
        PC->SetIgnoreMoveInput(true);
        PC->SetIgnoreLookInput(true);
        PC->bShowMouseCursor = true;
    }

    if (GetCharacterMovement())
    {
        GetCharacterMovement()->DisableMovement();
        GetCharacterMovement()->StopMovementImmediately();
    }
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    if (DeathMontage && GetMesh())
    {
        UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
        if (AnimInst)
        {
            AnimInst->Montage_Play(DeathMontage);
        }
    }

    UWorld* W = GetWorld();
    if (W)
    {
        if (AGMB_TopDown* GM = Cast<AGMB_TopDown>(UGameplayStatics::GetGameMode(this)))
        {
            GM->EndGame(false);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("MainCharacter::Die - Could not cast GameMode to AGMB_TopDown"));
        }
    }

}

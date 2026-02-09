#include "GMB_TopDown.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "EndGameWidget.h"
#include "GameHUDWidget.h"
#include "WaveManager.h"
#include "SpawnPoint.h"
#include "GameFramework/PlayerController.h"
#include "EngineUtils.h"

AGMB_TopDown::AGMB_TopDown()
{
    CurrentScore = 0;
    ElapsedTime = 0.f;
    InGameWidgetInstance = nullptr;
    EndScreenWidgetInstance = nullptr;

    PrimaryActorTick.bCanEverTick = true;
}

void AGMB_TopDown::BeginPlay()
{
    Super::BeginPlay();

    UWorld* W = GetWorld();
    if (W && InGameWidgetClass)
    {
        InGameWidgetInstance = CreateWidget<UGameHUDWidget>(W, InGameWidgetClass);
        if (InGameWidgetInstance)
        {
            InGameWidgetInstance->AddToViewport();
            InGameWidgetInstance->SetScore(CurrentScore);
            InGameWidgetInstance->SetTimeSeconds(ElapsedTime);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("GMB_TopDown::BeginPlay - failed to create InGameWidgetInstance"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Verbose, TEXT("GMB_TopDown::BeginPlay - InGameWidgetClass not set or World is null"));
    }
}

void AGMB_TopDown::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    UWorld* W = GetWorld();
    if (W && !W->IsPaused())
    {
        ElapsedTime += DeltaSeconds;
        if (InGameWidgetInstance)
        {
            InGameWidgetInstance->SetTimeSeconds(ElapsedTime);
        }
    }
}

void AGMB_TopDown::AddScore(int32 Amount)
{
    CurrentScore += Amount;
    OnScoreChanged.Broadcast(CurrentScore);
    UE_LOG(LogTemp, Log, TEXT("Score: %d"), CurrentScore);

    if (InGameWidgetInstance)
    {
        InGameWidgetInstance->SetScore(CurrentScore);
    }
}

void AGMB_TopDown::EndGame(bool bPlayerWon)
{
    UE_LOG(LogTemp, Warning, TEXT("Game Ended. Score: %d  PlayerWon=%d"), CurrentScore, bPlayerWon);

    UWorld* W = GetWorld();
    if (!W)
    {
        UE_LOG(LogTemp, Error, TEXT("EndGame: GetWorld() == nullptr"));
        return;
    }

    for (TActorIterator<AWaveManager> It(W); It; ++It)
    {
        AWaveManager* WM = *It;
        if (WM)
        {
            UE_LOG(LogTemp, Log, TEXT("EndGame: Stopping WaveManager %s"), *WM->GetName());
            WM->StopWaves();
        }
    }

    for (TActorIterator<ASpawnPoint> It2(W); It2; ++It2)
    {
        ASpawnPoint* Sp = *It2;
        if (Sp)
        {
            UE_LOG(LogTemp, Log, TEXT("EndGame: Stopping Spawner %s"), *Sp->GetName());
            Sp->bEnabled = false;
            Sp->SetActorTickEnabled(false);
        }
    }
    if (InGameWidgetInstance)
    {
        InGameWidgetInstance->RemoveFromParent();
        InGameWidgetInstance = nullptr;
    }

    if (EndScreenWidgetClass)
    {
        if (EndScreenWidgetInstance)
        {
            EndScreenWidgetInstance->RemoveFromParent();
            EndScreenWidgetInstance = nullptr;
        }

        APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);

        EndScreenWidgetInstance = CreateWidget<UEndGameWidget>(W, EndScreenWidgetClass);
        if (EndScreenWidgetInstance)
        {
            EndScreenWidgetInstance->FinalScore = CurrentScore;
            EndScreenWidgetInstance->SetFinalScore(CurrentScore);

            EndScreenWidgetInstance->AddToViewport(100);
            UE_LOG(LogTemp, Log, TEXT("EndGame: Created EndGameWidget with score %d"), CurrentScore);

            if (PC)
            {
                PC->bShowMouseCursor = true;
                FInputModeUIOnly InputMode;
                InputMode.SetWidgetToFocus(EndScreenWidgetInstance->TakeWidget());
                InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
                PC->SetInputMode(InputMode);
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("EndGame: Failed to create EndScreenWidgetInstance."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("EndGame: EndScreenWidgetClass not set in GameMode."));
    }

    UGameplayStatics::SetGamePaused(W, true);
}

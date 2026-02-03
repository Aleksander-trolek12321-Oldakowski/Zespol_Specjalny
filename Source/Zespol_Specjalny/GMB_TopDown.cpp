#include "GMB_TopDown.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "MainCharacter.h"
#include "EngineUtils.h"
#include "WaveManager.h"
#include "SpawnPoint.h"
#include "GameFramework/PlayerController.h"

AGMB_TopDown::AGMB_TopDown()
{
    CurrentScore = 0;
    EndScreenWidgetInstance = nullptr;
}

void AGMB_TopDown::AddScore(int32 Amount)
{
    CurrentScore += Amount;
    UE_LOG(LogTemp, Log, TEXT("Score: %d"), CurrentScore);
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
            UE_LOG(LogTemp, Log, TEXT("GMB_TopDown: Disabled SpawnPoint %s"), *Sp->GetName());
        }
    }

    if (EndScreenWidgetClass)
    {
        if (EndScreenWidgetInstance)
        {
            EndScreenWidgetInstance->RemoveFromParent();
            EndScreenWidgetInstance = nullptr;
        }

        APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);

        EndScreenWidgetInstance = CreateWidget<UUserWidget>(W, EndScreenWidgetClass);
        if (EndScreenWidgetInstance)
        {
            EndScreenWidgetInstance->AddToViewport(100);

            UFunction* Func = EndScreenWidgetInstance->FindFunction(TEXT("SetFinalScore"));
            if (Func)
            {
                struct FParms { int32 Score; };
                FParms P;
                P.Score = CurrentScore;
                EndScreenWidgetInstance->ProcessEvent(Func, &P);
                UE_LOG(LogTemp, Log, TEXT("EndGame: Called SetFinalScore(%d) on EndScreen widget."), CurrentScore);
            }
            else
            {
                UE_LOG(LogTemp, Log, TEXT("EndGame: EndScreen widget has no SetFinalScore function - ensure UMG exposes score some other way."));
            }

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
        UE_LOG(LogTemp, Warning, TEXT("EndGame: EndScreenWidgetClass not set in GameMode. No end-screen will be shown."));
    }

    UGameplayStatics::SetGamePaused(W, true);
}

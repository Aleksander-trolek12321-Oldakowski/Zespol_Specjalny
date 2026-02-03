#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GMB_TopDown.generated.h"

class UUserWidget;
class AWaveManager;
class AEnemySpawner;

UCLASS()
class ZESPOL_SPECJALNY_API AGMB_TopDown : public AGameModeBase
{
    GENERATED_BODY()

public:
    AGMB_TopDown();

    UFUNCTION(BlueprintCallable)
    void AddScore(int32 Amount);

    UFUNCTION(BlueprintCallable)
    void EndGame(bool bPlayerWon);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 CurrentScore;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> EndScreenWidgetClass;

protected:
    UPROPERTY()
    UUserWidget* EndScreenWidgetInstance;
};

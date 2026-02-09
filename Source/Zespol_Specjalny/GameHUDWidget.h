#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "GameHUDWidget.generated.h"

UCLASS()
class ZESPOL_SPECJALNY_API UGameHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="HUD")
    void SetScore(int32 NewScore);

    UFUNCTION(BlueprintCallable, Category="HUD")
    void SetTimeSeconds(float TimeSeconds);

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ScoreTextBlock;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* TimeTextBlock;

private:
    void UpdateScoreText();
    void UpdateTimeText();

    int32 CurrentScore = 0;
    float CurrentTimeSeconds = 0.f;
};

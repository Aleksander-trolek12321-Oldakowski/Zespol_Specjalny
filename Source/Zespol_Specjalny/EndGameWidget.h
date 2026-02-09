#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "EndGameWidget.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ZESPOL_SPECJALNY_API UEndGameWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Game Over")
    void SetFinalScore(int32 Score);

    UFUNCTION(BlueprintImplementableEvent, Category = "Game Over")
    void OnScoreUpdated();

    UFUNCTION(BlueprintCallable, Category = "Game Over")
    void SetScoreTextBlock(UTextBlock* TextBlock);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Game Over")
    int32 GetFinalScore() const { return FinalScore; }

protected:
    virtual void NativeConstruct() override;

    virtual void NativeOnInitialized() override;

    void UpdateScoreText();

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Over", meta = (ExposeOnSpawn = "true"))
    int32 FinalScore;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Game Over")
    UTextBlock* ScoreTextBlock;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Over")
    FName ScoreTextBlockName;
};
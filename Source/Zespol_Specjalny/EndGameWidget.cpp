#include "EndGameWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"
#include "Engine/Engine.h"

void UEndGameWidget::SetFinalScore(int32 Score)
{
    FinalScore = Score;
    
    UpdateScoreText();
    
    OnScoreUpdated();
}

void UEndGameWidget::SetScoreTextBlock(UTextBlock* TextBlock)
{
    ScoreTextBlock = TextBlock;
    UpdateScoreText();
}

void UEndGameWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!ScoreTextBlock && !ScoreTextBlockName.IsNone())
    {
        UWidget* Found = GetWidgetFromName(ScoreTextBlockName);
        if (Found)
        {
            ScoreTextBlock = Cast<UTextBlock>(Found);
            if (!ScoreTextBlock)
            {
                UE_LOG(LogTemp, Warning, TEXT("EndGameWidget: Found widget '%s' but it is not a UTextBlock."), *ScoreTextBlockName.ToString());
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("EndGameWidget: No widget named '%s' found in widget tree."), *ScoreTextBlockName.ToString());
        }
    }

    UpdateScoreText();
}

void UEndGameWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    UpdateScoreText();
}

void UEndGameWidget::UpdateScoreText()
{
    if (ScoreTextBlock)
    {
        ScoreTextBlock->SetText(FText::AsNumber(FinalScore));
    }
}
#include "GameHUDWidget.h"
#include "Components/TextBlock.h"

void UGameHUDWidget::SetScore(int32 NewScore)
{
    CurrentScore = NewScore;
    UpdateScoreText();
}

void UGameHUDWidget::SetTimeSeconds(float TimeSeconds)
{
    CurrentTimeSeconds = TimeSeconds;
    UpdateTimeText();
}

void UGameHUDWidget::UpdateScoreText()
{
    if (ScoreTextBlock)
    {
        ScoreTextBlock->SetText(FText::AsNumber(CurrentScore));
    }
}

void UGameHUDWidget::UpdateTimeText()
{
    if (TimeTextBlock)
    {
        int32 TotalSec = FMath::Max(0, FMath::FloorToInt(CurrentTimeSeconds));
        int32 Minutes = TotalSec / 60;
        int32 Seconds = TotalSec % 60;
        FText Formatted = FText::FromString(FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds));
        TimeTextBlock->SetText(Formatted);
    }
}

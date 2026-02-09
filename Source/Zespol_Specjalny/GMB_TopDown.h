#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UObject/NameTypes.h"
#include "GMB_TopDown.generated.h"

class UEndGameWidget;
class UGameHUDWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChangedSignature, int32, NewScore);

UCLASS()
class ZESPOL_SPECJALNY_API AGMB_TopDown : public AGameModeBase
{
    GENERATED_BODY()

public:
    AGMB_TopDown();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    UFUNCTION(BlueprintCallable)
    void AddScore(int32 Amount);

    UFUNCTION(BlueprintCallable)
    void EndGame(bool bPlayerWon);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 CurrentScore;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UGameHUDWidget> InGameWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UEndGameWidget> EndScreenWidgetClass;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnScoreChangedSignature OnScoreChanged;

protected:
    UPROPERTY()
    UGameHUDWidget* InGameWidgetInstance;

    UPROPERTY()
    UEndGameWidget* EndScreenWidgetInstance;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float ElapsedTime;
};

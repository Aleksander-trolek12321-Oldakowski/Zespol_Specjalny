#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/CheckBox.h"
#include "Components/VerticalBox.h"
#include "MainMenuWidget.generated.h"

UCLASS()
class ZESPOL_SPECJALNY_API UMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    UPROPERTY(meta=(BindWidget))
    UButton* NewGameButton;

    UPROPERTY(meta=(BindWidget))
    UButton* OptionsButton;

    UPROPERTY(meta=(BindWidget))
    UButton* CreditsButton;

    UPROPERTY(meta=(BindWidget))
    UButton* QuitButton;

    UPROPERTY(meta=(BindWidget))
    USlider* MasterVolumeSlider;

    UPROPERTY(meta=(BindWidget))
    UCheckBox* FullscreenCheckBox;

protected:
    UFUNCTION()
    void OnNewGameClicked();

    UFUNCTION()
    void OnOptionsClicked();

    UFUNCTION()
    void OnCreditsClicked();

    UFUNCTION()
    void OnQuitClicked();

    UFUNCTION()
    void OnMasterVolumeChanged(float Value);

    UFUNCTION()
    void OnFullscreenChanged(bool bChecked);
};

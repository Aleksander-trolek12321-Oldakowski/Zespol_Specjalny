#include "MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MyGameInstance.h"
#include "Components/TextBlock.h"

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (NewGameButton) NewGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnNewGameClicked);
    if (OptionsButton) OptionsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnOptionsClicked);
    if (CreditsButton) CreditsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnCreditsClicked);
    if (QuitButton) QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClicked);

    if (MasterVolumeSlider) MasterVolumeSlider->OnValueChanged.AddDynamic(this, &UMainMenuWidget::OnMasterVolumeChanged);
    if (FullscreenCheckBox) FullscreenCheckBox->OnCheckStateChanged.AddDynamic(this, &UMainMenuWidget::OnFullscreenChanged);

    if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
    {
        if (MasterVolumeSlider) MasterVolumeSlider->SetValue(GI->GetMasterVolume());
        if (FullscreenCheckBox) FullscreenCheckBox->SetIsChecked(GI->GetFullScreen());
    }
}

void UMainMenuWidget::OnNewGameClicked()
{
    UGameplayStatics::OpenLevel(this, FName(TEXT("GameplayMap")));
}

void UMainMenuWidget::OnOptionsClicked()
{
}

void UMainMenuWidget::OnCreditsClicked()
{
}

void UMainMenuWidget::OnQuitClicked()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UMainMenuWidget::OnMasterVolumeChanged(float Value)
{
    if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
    {
        GI->SetMasterVolume(Value);
        GI->SaveSettings();
    }

}

void UMainMenuWidget::OnFullscreenChanged(bool bChecked)
{
    if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
    {
        GI->SetFullScreen(bChecked);
        GI->SaveSettings();
    }
}

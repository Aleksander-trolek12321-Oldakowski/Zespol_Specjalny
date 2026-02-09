#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "GameFramework/GameUserSettings.h"

void UMyGameInstance::Init()
{
    Super::Init();
    LoadSettings();
}

void UMyGameInstance::EnsureSave()
{
    if (!LoadedSave)
    {
        LoadedSave = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
    }
}

bool UMyGameInstance::LoadSettings()
{
    if (UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
    {
        LoadedSave = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
    }
    else
    {
        EnsureSave();
    }

    if (LoadedSave)
    {
        if (GEngine)
        {
            if (UGameUserSettings* S = GEngine->GetGameUserSettings())
            {
                S->SetFullscreenMode(LoadedSave->bFullScreen ? EWindowMode::Fullscreen : EWindowMode::Windowed);
                S->ApplySettings(false);
            }
        }
        return true;
    }
    return false;
}

bool UMyGameInstance::SaveSettings()
{
    EnsureSave();
    return UGameplayStatics::SaveGameToSlot(LoadedSave, SlotName, UserIndex);
}

float UMyGameInstance::GetMasterVolume() const
{
    return LoadedSave ? LoadedSave->MasterVolume : 1.0f;
}

void UMyGameInstance::SetMasterVolume(float NewVolume)
{
    EnsureSave();
    LoadedSave->MasterVolume = FMath::Clamp(NewVolume, 0.f, 1.f);

}

bool UMyGameInstance::GetFullScreen() const
{
    return LoadedSave ? LoadedSave->bFullScreen : true;
}

void UMyGameInstance::SetFullScreen(bool bFull)
{
    EnsureSave();
    LoadedSave->bFullScreen = bFull;
    if (GEngine)
    {
        if (UGameUserSettings* S = GEngine->GetGameUserSettings())
        {
            S->SetFullscreenMode(bFull ? EWindowMode::Fullscreen : EWindowMode::Windowed);
            S->ApplySettings(false);
        }
    }
}

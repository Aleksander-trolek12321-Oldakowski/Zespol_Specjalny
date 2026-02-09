#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MySaveGame.h"
#include "MyGameInstance.generated.h"

UCLASS()
class ZESPOL_SPECJALNY_API UMyGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    virtual void Init() override;

    UFUNCTION(BlueprintCallable, Category="Save")
    bool LoadSettings();

    UFUNCTION(BlueprintCallable, Category="Save")
    bool SaveSettings();

    UFUNCTION(BlueprintCallable, Category="Settings")
    float GetMasterVolume() const;

    UFUNCTION(BlueprintCallable, Category="Settings")
    void SetMasterVolume(float NewVolume);

    UFUNCTION(BlueprintCallable, Category="Settings")
    bool GetFullScreen() const;

    UFUNCTION(BlueprintCallable, Category="Settings")
    void SetFullScreen(bool bFull);

protected:
    UPROPERTY()
    UMySaveGame* LoadedSave = nullptr;

    FString SlotName = TEXT("PlayerSettings");
    uint32 UserIndex = 0;

    void EnsureSave();
};

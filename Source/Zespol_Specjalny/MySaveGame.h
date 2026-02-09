#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

UCLASS()
class ZESPOL_SPECJALNY_API UMySaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, Category="Save")
    float MasterVolume = 1.0f;

    UPROPERTY(VisibleAnywhere, Category="Save")
    bool bFullScreen = true;
};

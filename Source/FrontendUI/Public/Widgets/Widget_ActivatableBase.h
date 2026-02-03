// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Widget_ActivatableBase.generated.h"

class AFrontendPlayerController;

UCLASS(Abstract, BlueprintType, meta=(DisableNaiveTick))
class FRONTENDUI_API UWidget_ActivatableBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	// Função pura para acessar o Player Controller a partir da Blueprint
	UFUNCTION(BlueprintPure)
	AFrontendPlayerController* GetOwningFrontendPC();

private:
	// Cache da referência do PlayerController dono do widget
	TWeakObjectPtr<AFrontendPlayerController> CachedOwningFrontendPC;
};

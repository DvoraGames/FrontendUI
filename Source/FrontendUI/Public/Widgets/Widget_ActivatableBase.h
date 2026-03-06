// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Widget_ActivatableBase.generated.h"

/**
* UWidget_ActivatableBase
*
* Widget base do Frontend - herda de UCommonActivatableWidget e adiciona
* acesso facilitado ao AFrontendPlayerController dono do widget via cache lazy.
*
* Todos os widgets ativaveis do Frontend devem herdar desta classe.
*/
class AFrontendPlayerController;

UCLASS(Abstract, BlueprintType, meta=(DisableNaiveTick))
class FRONTENDUI_API UWidget_ActivatableBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	// Retorna o AFrontendPlayerController dono deste widget.
	UFUNCTION(BlueprintPure)
	AFrontendPlayerController* GetOwningFrontendPC();

private:
	// Cache fraco do PlayerController para evitar cast a cada chamada.
	TWeakObjectPtr<AFrontendPlayerController> CachedOwningFrontendPC;
};

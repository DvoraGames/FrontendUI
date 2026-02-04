// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "Widget_OptionsScreen.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNaiveTick))
class FRONTENDUI_API UWidget_OptionsScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()
	
protected:
	//~ Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	//~ End UUserWidget Interface
	
private:
	// Função para quando a ação Reset for executada
	void OnResetBoundActionTriggered();
	
	// Propriedade para definir do Input Action da ação de reset.
	UPROPERTY(EditDefaultsOnly, Category="Frontend Options Screen")
	UInputAction* ResetAction;
	
	// Armazena o handle da ação de reset para gerenciamento manual.
	FUIActionBindingHandle ResetActionHandle;
};

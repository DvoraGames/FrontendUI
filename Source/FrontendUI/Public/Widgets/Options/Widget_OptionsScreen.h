// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "Widget_OptionsScreen.generated.h"

class UFrontendTabListWidgetBase;
class UOptionsDataRegistry;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNaiveTick))
class FRONTENDUI_API UWidget_OptionsScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()
	
protected:
	//~ Begin UUserWidget Interface
	// Função chamada na Inicialização do Widget
	virtual void NativeOnInitialized() override;
	//~ End UUserWidget Interface
	
	//~ Begin UCommonActivatableWidget Interface
	// Função chamada na Ativação do Widget
	virtual void NativeOnActivated() override;
	//~ End UCommonActivatableWidget Interface

private:
	// Função que verifica e pega o Registry se ele existir, caso contrario cria um e Chama InitOptionsDataRegistry()
	UOptionsDataRegistry* GetOrCreateDataRegistry();
	
	// Callback do input Reset (tecla/botão) - limpa tudo pras configs default
	void OnResetBoundActionTriggered();
	
	// Callback quando usuário clica numa aba - carrega conteúdo da aba selecionada
	UFUNCTION()
	void OnOptionsTabSelected(FName TabID);
	
	/***** Bound Widgets *****/
	UPROPERTY(meta=(BindWidget))
	// Widget Blueprint vinculado - onde os botões das abas vão aparecer (HorizontalBox interno)
	UFrontendTabListWidgetBase* TabListWidget_OptionsTabs;
	/***** Bound Widgets *****/

	UPROPERTY(Transient)
	// Registry criado POR ESSA tela (ou reutilizado). Vive enquanto tela existir (Transient)
	UOptionsDataRegistry* CreatedOwningDataRegistry;
	
    // Input Action da ação de reset - "Reset Settings" (X no gamepad, ESC+Reset no teclado)
	UPROPERTY(EditDefaultsOnly, Category="Frontend Options Screen")
	UInputAction* ResetAction;
	
	// Armazena o handle da ação de reset para gerenciamento manual.
	FUIActionBindingHandle ResetActionHandle;
};

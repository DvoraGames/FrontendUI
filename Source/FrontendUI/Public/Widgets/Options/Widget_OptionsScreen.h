// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "Widget_OptionsScreen.generated.h"

class UWidget_OptionsDetailsView;
class UFrontendCommonListView;
class UFrontendTabListWidgetBase;
class UOptionsDataRegistry;

/*
 * Tela principal de opções/configurações do jogo.
 * Gerencia a navegação entre abas (Gameplay, Audio, Video, etc.) usando um TabList e exibe as configurações da aba selecionada em uma ListView.
 * Instancia e gerencia um UOptionsDataRegistry para fornecer os dados das opções.
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
	// Função chamada na Desativação do Widget
	virtual void NativeOnDeactivated() override;
	//~ End UCommonActivatableWidget Interface

private:
	// Função que retorna a instância atual do Registry de opções. Se não existir, cria uma nova instância e chama sua inicialização. 
	UOptionsDataRegistry* GetOrCreateDataRegistry();
	
    // Handler acionado pelo Input Action de Reset. Restaura todas as configurações da aba atual para o padrão.
	void OnResetBoundActionTriggered();
	
	// Handler acionado quando o usuário seleciona uma aba. Consulta o Registry pelo conteúdo da aba (TabID) e popula a ListView principal. 
	UFUNCTION()
	void OnOptionsTabSelected(FName TabID);
	
	// Handler chamado quando um item da ListView é hovereado. Usado para atualizar feedback visual do item em foco.
	void OnListViewItemHovered(UObject* InHoveredItem, bool bIsHovered);
	
	// Handler chamado quando um item da ListView é selecionado. 
	/* Pode ser usado para expandir detalhes ou acionar lógica específica do tipo de opção selecionada. */
	void OnListViewItemSelected(UObject* InSelectedItem);
	
	FString TryGetEntryWidgetClassName(UObject* InOwningListItem) const;
	
	/***** Bound Widgets *****/
	// Widget vinculado da box que define onde os botões das abas vão aparecer (HorizontalBox interno)
	UPROPERTY(meta=(BindWidget))
	UFrontendTabListWidgetBase* TabListWidget_OptionsTabs;
	
	// Widget vinculado da Lista que exibe as opções (carrosséis, sliders, etc.) da aba selecionada.
	UPROPERTY(meta=(BindWidget))
	UFrontendCommonListView* CommonListView_OptionsList;
	
	// Widget vinculado responsavel por mostrar informações de cada opção
	UPROPERTY(meta=(BindWidget))
	UWidget_OptionsDetailsView* DetailsView_ListEntryInfo;
	/***** Bound Widgets *****/

	// Registry de dados instanciado por esta tela. Mantém o estado temporário e a hierarquia das opções enquanto a tela estiver viva. 
	UPROPERTY(Transient)
	UOptionsDataRegistry* CreatedOwningDataRegistry;
	
    // Input Action da ação de reset - "Reset Settings" (X no gamepad, ESC+Reset no teclado)
	UPROPERTY(EditDefaultsOnly, Category="Frontend Options Screen")
	UInputAction* ResetAction;
	
	// Handle salvo internamente para gerenciar o ciclo de vida do Input Action de Reset.
	FUIActionBindingHandle ResetActionHandle;
};

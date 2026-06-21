// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "Widget_OptionsScreen.generated.h"

class UFrontendCommonTreeView;
class UListDataObject_Base;
class UWidget_OptionsDetailsView;
class UFrontendCommonListView;
class UFrontendTabListWidgetBase;
class UOptionsDataRegistry;

/**
 * UWidget_OptionsScreen
 *
 * Tela principal de opções/configurações do jogo.
 * Gerencia a navegação entre abas (Gameplay, Audio, Video, etc.) usando um TabList e exibe as configurações da aba 
 * selecionada em uma ListView. 
 * 
 * Instancia e gerencia um UOptionsDataRegistry para fornecer os dados das opções enquanto a tela estiver viva.
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNaiveTick))
class FRONTENDUI_API UWidget_OptionsScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()
	
protected:
	//~ Begin UUserWidget Interface
	// Chamado uma única vez na inicialização do widget - registra bindings de input e delegates.
	virtual void NativeOnInitialized() override;
	//~ End UUserWidget Interface
	
	//~ Begin UCommonActivatableWidget Interface
    // Registra as abas disponíveis ao ativar a tela.
	virtual void NativeOnActivated() override;
	
	// Chamado ao desativar a tela — força o salvamento das configurações no .ini.
	virtual void NativeOnDeactivated() override;
	
	// Retorna a Entry atualmente selecionada como alvo de foco do CommonUI.
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	//~ End UCommonActivatableWidget Interface
	
	// Busca e Retorna o primeiro DataObject selecionável da aba atual.
	static UListDataObject_Base* FindFirstSelectableItem(const TArray<UObject*>& Items);
	
private:
	// Retorna o registry de opções, criando-o na primeira chamada.
	UOptionsDataRegistry* GetOrCreateDataRegistry();
	
	// Handler do Input Action de Reset — restaura as configurações da aba atual para o padrão.
	void OnResetBoundActionTriggered();
	
	// Handler que atualiza a árvore e os dados da aba selecionada.
	UFUNCTION()
	void OnOptionsTabSelected(FName TabID);
	
	// Handler chamado quando o estado de hover de uma Entry muda.
	void OnListItemHovered(UObject* InHoveredItem, bool bIsHovered) const;
	
	// Handler chamado quando um item é selecionado.
	/* Pode ser usado para expandir detalhes ou acionar lógica específica do tipo de opção selecionada. */
	void OnListItemSelected(UObject* InSelectedItem) const;
	
	// Tenta obter o nome da classe do Entry Widget associado ao item.
	/* Usado pela DetailsView para customizar o conteúdo por tipo de entry. Retorna "Entry Widget Not Valid" se o item 
	não estiver visível na ListView. */
	FString TryGetEntryWidgetClassName(UObject* InOwningListItem) const;
	
	// Atualiza o controle de itens resetáveis quando um DataObject for modificado.
	/* Gerencia o ResettableDataArray e o binding do ResetAction: adiciona o binding se houver dados resetáveis, remove 
	se não houver. */
	void OnListListDataModified(UListDataObject_Base* ModifiedData, EOptionsListDataModifyReason ModifiedReason);
	
	// Fornece os filhos de uma Entry Collection para a TreeView.
	UFUNCTION()
	void HandleGetItemChildren(UObject* InItem, TArray<UObject*>& OutChildren);
	
	// ----------------------------------------------------------
	// Bound Widget
	// ----------------------------------------------------------
	
	// Widget TabList vinculado - botões de navegação entre abas (Gameplay, Audio, etc.).
	UPROPERTY(meta=(BindWidget))
	UFrontendTabListWidgetBase* TabListWidget_OptionsTabs;
	
	// Widget TreeView vinculado - exibe as opções (carrosseis, sliders, etc.) de forma hierarquica da aba selecionada.
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UFrontendCommonTreeView* CommonTreeView_OptionsList;

	// Details view usada para exibir informações da opção atual.
	UPROPERTY(meta=(BindWidget))
	UWidget_OptionsDetailsView* DetailsView_ListEntryInfo;
	
	// ----------------------------------------------------------
	// Data and Input
	// ----------------------------------------------------------

	// Registry de dados instanciado por esta tela. Mantém o estado e a hierarquia das opções enquanto a tela estiver viva.
	UPROPERTY(Transient)
	UOptionsDataRegistry* CreatedOwningDataRegistry;
	
	/* Array de DataObjects que podem ser resetados para o valor padrão. 
	Preenchido ao trocar de aba e atualizado conforme o usuário modifica opções. */
	UPROPERTY(Transient)
	TArray<UListDataObject_Base*> ResettableDataArray;
	
    // Input Action do reset - (X no gamepad, ESC no teclado). Configurado no Blueprint filho via EditDefaultsOnly.
	UPROPERTY(EditDefaultsOnly, Category="Frontend Options Screen")
	UInputAction* ResetAction;
	
	// Handle do binding de input do Reset — usado para adicionar/remover dinamicamente.
	FUIActionBindingHandle ResetActionHandle;
	
	// Flag que indica se um reset está em andamento — evita reações do delegate durante o reset.
	bool bIsResetingData = false;
};

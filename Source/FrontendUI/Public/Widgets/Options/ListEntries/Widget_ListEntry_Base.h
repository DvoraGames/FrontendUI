// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "Widget_ListEntry_Base.generated.h"

class UWidget_ToggleAction;
class UInputAction;
class UToggleActionButton;
class UWidget_EntryRow;
class UListDataObject_Base;
class UCommonTextBlock;

/**
* UWidget_ListEntry_Base
*
* Widget base para as entries da lista de opções.
* Implementa a interface IUserObjectListEntry para receber um UListDataObject_Base e refletir seus dados visualmente.
*
* Subclasses como Carousel e Slider herdam daqui para adicionar interações específicas.
*/
UCLASS(Abstract, BlueprintType, meta=(DisableNaiveTick))
class FRONTENDUI_API UWidget_ListEntry_Base : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	// Evento Blueprint disparado quando o hover do item muda.
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "On Entry Widget Hovered"))
	void BP_OnItemHovered(bool bIsHovered, bool bIsEntryWidgetStillSelected);
	
	// Repassa o estado de hover para o Blueprint.
	void NativeOnItemHovered(bool bIsHovered);

protected:
	// Retorna o widget que deve receber foco ao navegar com Gamepad.
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "Get Widget To Focus For Gamepad"))
	UWidget* BP_GetWidgetToFocusForGamepad() const;
	
	//~ Begin IUserObjectListEntry Interface
	// Recebe o UObject generico da lista, converte para DataObject e inicia a configuração da entry.
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	
	// Limpa o estado visual quando a lista libera a entry.
	virtual void NativeOnEntryReleased() override;
	
	// Reage à mudança de seleção do item, atualizando row e toggle action.
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
	//~ End IUserObjectListEntry Interface
	
	//~ Begin UUserWidget Interface
	// Faz os binds iniciais dos botões e do Rotator quando a entry é criada.
	virtual void NativeOnInitialized() override;
	
	// Redireciona o foco para um widget interno.
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	
	// Atualiza o estado visual de expansão da entry.
	virtual void NativeOnItemExpansionChanged(bool bIsExpanded) override;
	//~ End UUserWidget Interface

	//Chamado logo após a Lista injetar os dados nesta entrada.
	/* Na classe base, atualiza o nome exibido e faz o bind no delegate de modificação.
	 * Subclasses devem chamar Super e implementar suas formatações específicas. */
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject);
	
    // Handler chamado quando o DataObject associado sofre alguma modificação.
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, 
		EOptionsListDataModifyReason ModifyReason);
	
	/* Força este entry a tornar-se o item selecionado na ListView. Útil para entries que possuem interações internas 
	(ex: botões	de carrossel) e precisam se autosselecionar ao serem clicados. */
	void SelectThisEntryWidget() const;
	
	// Sincroniza o estado expandido do item no DataObject e na TreeView.
	void SyncTreeExpansion(UListDataObject_Base* InDataObject, const bool bNewExpansion) const;
	
	// Associa o DataObject dono desta entry.
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UListDataObject_Base* OwningListDataObject = nullptr;
	
	// Executa a ação de toggle associada ao DataObject desta entry.
	virtual void OnToggleActionClicked() const;
	
private:
	// Retorna a profundidade hierárquica do DataObject dono.
	UFUNCTION(BlueprintPure, Category="Options")
	int32 GetOwningDataHierarchyDepth() const;
	
	// Atualiza apenas o estado ativo/inativo visual do botão de toggle.
	void RefreshToggleActionVisual() const;
	
	// Atualiza visibilidade, estado e hint de input do toggle action completo.
	void RefreshToggleActionPresentation() const;
	
	// Registra o input action de toggle enquanto esta entry estiver selecionada.
	void RegisterToggleActionBinding();
	
	// Remove o binding do input action de toggle.
	void UnregisterToggleActionBinding();
	
	// ----------------------------------------------------------
	// Bound Widget
	// ----------------------------------------------------------
	
	// Texto que exibe o nome da configuração nesta entry. (ex: "Qualidade Gráfica", "Volume").
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess = "true"))
	UCommonTextBlock* CommonText_SettingDisplayName;
	
	// Widget auxiliar da row responsável por indentação e expansão visual.
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	UWidget_EntryRow* WBP_Entry_RowLayout;
	
	// Widget opcional que exibe o botão/hint de ação de toggle (Mute, Link, etc).
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess = "true"))
	UWidget_ToggleAction* WBP_ToggleAction;
	
	// ----------------------------------------------------------
	// Data and Input
	// ----------------------------------------------------------
	
	// Input action usado para acionar o toggle via input dedicado (ex: botão do gamepad).
	UPROPERTY(EditDefaultsOnly, Category="Frontend|Input")
	UInputAction* ToggleInputAction;

	// Handle do binding do input action de toggle, usado para registrar/remover.
	FUIActionBindingHandle ToggleEntryActionHandle;
};

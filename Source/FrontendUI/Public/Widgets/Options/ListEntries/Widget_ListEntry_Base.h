// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "Widget_ListEntry_Base.generated.h"

class UListDataObject_Base;
class UCommonTextBlock;

/**
* UWidget_ListEntry_Base
*
* Widget base para todas as entradas exibidas dentro da ListView de opções.
* Implementa a interface IUserObjectListEntry para receber um UListDataObject_Base
* e refletir seus dados visualmente.
*
* Subclasses como Carousel e Slider herdam daqui para adicionar interações específicas.
*/
UCLASS(Abstract, BlueprintType, meta=(DisableNaiveTick))
class FRONTENDUI_API UWidget_ListEntry_Base : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	// Evento Blueprint disparado quando o hover do item muda.
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "On Item Hovered"))
	void BP_OnItemHovered(bool bIsHovered, bool bIsEntryWidgetStillSelected);
	
	// Repassa o estado de hover para o Blueprint, com o estado atual de seleção.
	void NativeOnItemHovered(bool bIsHovered);

protected:
	// Retorna o widget que deve receber foco ao navegar com Gamepad.
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "Get Widget To Focus For Gamepad"))
	UWidget* BP_GetWidgetToFocusForGamepad() const;
	
	//~ Begin IUserObjectListEntry Interface
	// Recebe o UObject da ListView, converte para DataObject e inicia a configuração visual desta entry.
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	
	// Chamado quando esta entry é liberada pela ListView - usado para limpeza de estado visual transitório.
	virtual void NativeOnEntryReleased() override;
	//~ End IUserObjectListEntry Interface
	
	//~ Begin UUserWidget Interface
	// Redireciona o foco para um widget interno apropriado ao usar Gamepad.
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	//~ Begin UUserWidget Interface

	//Chamado logo após a ListView injetar os dados nesta entrada.
	/* Na classe base, atualiza o nome exibido e faz o bind no delegate de modificação.
	 * Subclasses devem chamar Super e implementar suas formatações específicas. */
	virtual  void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject);
	
    // Handler chamado quando o DataObject associado sofre alguma modificação.
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, 
		EOptionsListDataModifyReason ModifyReason);
	
	/* Força este entry a tornar-se o item selecionado na ListView. Útil para entries que possuem interações internas 
	(ex: botões	de carrossel) e precisam se auto-selecionar ao serem clicados. */
	void SelectThisEntryWidget() const;
	
private:
	
	// ----------------------------------------------------------
	// Bound Widget
	// ----------------------------------------------------------
	
	// Texto que exibe o nome da configuração nesta entry. (ex: "Qualidade Gráfica", "Volume").
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess = "true"))
	UCommonTextBlock* CommonText_SettingDisplayName;


};

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
 * Widget base para todas as entradas exibidas dentro da ListView de opções.
 * 
 * Implementa a interface IUserObjectListEntry para receber um UListDataObject_Base
 * (os dados) e refleti-los visualmente (ex: atualizando o nome da configuração).
 * Subclasses (como Carousel, Slider) herdam daqui para adicionar interações específicas.
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNaiveTick))
class FRONTENDUI_API UWidget_ListEntry_Base : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	// Evento Blueprint chamado quando o estado de hover muda. Implementar no Blueprint filho para atualizar feedback visual
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "On Item Hovered"))
	void BP_OnItemHovered(bool bIsHovered, bool bIsEntryWidgetStillSelected);
	
	// Chamado externamente para notificar que o estado de hover deste item mudou.
	// Delega ao evento Blueprint BPOnItemHovered, passando também se o item está atualmente selecionado.
	void NativeOnItemHovered(bool bIsHovered);
	
protected:
	//~ Begin IUserObjectListEntry Interface
	/* Função nativa da ListView disparada quando esta entrada recebe seu objeto de dados. 
	Converte o UObject genérico e repassa para OnOwningListDataObjectSet. */
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	//~ End IUserObjectListEntry Interface
	
	/*
	 * Chamado logo após a ListView injetar os dados nesta entrada.
	 * Na classe base, atualiza o nome exibido e faz o bind no delegate de modificação.
	 * Subclasses DEVEM chamar Super e implementar suas formatações específicas.
	 */
	virtual  void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject);
	
	// Handler acionado sempre que o DataObject vinculado a esta entrada sofrer alterações
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, 
		EOptionsListDataModifyReason ModifyReason);
	
	/* Força este entry a tornar-se o item selecionado na ListView. Útil para entries que possuem interações internas 
	(ex: botões	de carrossel) e precisam se auto-selecionar ao serem clicados. */
	void SelectThisEntryWidget() const;
	
private:
	/***** Bind Widgets *****/
	// Widget de Texto que exibe o nome da configuração (ex: "Qualidade Gráfica", "Volume").
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess = "true"))
	UCommonTextBlock* CommonText_SettingDisplayName;
	/***** Bind Widgets *****/


};

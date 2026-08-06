// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Widget_EntryRow.generated.h"


class UToggleActionButton;
class UFrontendCommonTreeView;
class UListDataObject_Base;

/**
 * UWidget_EntryRow
 * 
 * Widget Componente da row da entry responsável por montar a indentação visual
 * e controlar a expansão de itens que possuem filhos dentro do TreeView. 
 */
UCLASS()
class FRONTENDUI_API UWidget_EntryRow : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	// Monta a indentação visual da row com base na profundidade hierárquica do DataObject.
	UFUNCTION(BlueprintCallable)
	void BuildIndent (UListDataObject_Base* InEntryDataObject);
	
	// Evento em Blueprint chamado para aplicar a indentação visual da row.
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Set Indent")
	void BP_SetIndent ();
	
	// Evento em Blueprint chamado quando o estado de expansão do item é alterado.
	UFUNCTION(BlueprintImplementableEvent, DisplayName="On Item Expansion Changed")
	void BP_OnItemExpansionChanged(bool bCanExpand, bool bIsExpanded);
	
	// Solicita a troca do estado de expansão do item.
	UFUNCTION(BlueprintCallable)
	void RequestToggleExpansion() const;
	
	// Evento em Blueprint chamado quando esta row é selecionada ou desselecionada.
	UFUNCTION(BlueprintImplementableEvent, DisplayName="On Owning Entry Selected")
	void BP_OnOwningEntrySelected(bool bIsSelected);
	
	// Evento em Blueprint chamado quando o hover sobre esta row é alterado.
	UFUNCTION(BlueprintImplementableEvent, DisplayName="On Owning Entry Hovered")
	void BP_OnOwningEntryHovered(bool bIsHovered, bool bEntryStillSelected);
	
	// Retorna true se este item pode ser expandido manualmente.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool BP_CanManualExpand() const;
	
	// Atualiza o visual do botão de expansão conforme o estado atual.
	void RefreshExpansionButtonVisual() const;
	
protected:
	// Faz o bind inicial dos eventos do botão de expansão.
	virtual void NativeOnInitialized() override;
	
private:	
	// DataObject associado a esta row.
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UListDataObject_Base* EntryDataObject;
	
	// Botão responsável por expandir/recolher os filhos desta row.
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UToggleActionButton* ToggleButton_Expansion;
};

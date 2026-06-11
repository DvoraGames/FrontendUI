// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Widget_EntryRow.generated.h"


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
	void BP_OnItemExpansionChanged(bool bIsExpandable, bool bIsExpanded);
	
	// Solicita a troca do estado de expansão do item.
	UFUNCTION(BlueprintCallable)
	void RequestToggleExpansion() const;
	
private:	
	// DataObject associado a esta row.
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UListDataObject_Base* EntryDataObject;
};

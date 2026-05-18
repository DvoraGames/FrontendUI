// DvoraGames All Rights Reserved


#include "Widgets/Components/FrontendCommonTreeView.h"

#include "Editor/WidgetCompilerLog.h"
#include "Widgets/Options/DataAsset_DataListEntryMapping.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"

UUserWidget& UFrontendCommonTreeView::OnGenerateEntryWidgetInternal(UObject* Item,
	TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	// Em design time não há mapeamento disponível - usa o comportamento padrão
	if (IsDesignTime())
	{
		return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	}
	
	// Consulta o DataAsset para encontrar o widget correspondente ao tipo do DataObject
	if (TSubclassOf<UWidget_ListEntry_Base> FoundWidgetClass = 
		DataListEntryMapping->FindEntryWidgetClassByDataObject(CastChecked<UListDataObject_Base>(Item)))
	{
		// Widget encontrado no mapeamento - gera a entrada tipada com o widget correto
		return GenerateTypedEntry<UWidget_ListEntry_Base>(FoundWidgetClass, OwnerTable);
	}
	
	// Fallback - nenhum mapeamento encontrado, usa o widget padrão da lista
	return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
}

void UFrontendCommonTreeView::SetTreeViewItems(const TArray<UListDataObject_Base*>& InTreeItems)
{
	// Filtra apenas itens válidos antes de enviar para o TreeView.
	TArray<UObject*> TreeItems;
	
	// Percorre todos os itens recebidos para o TreeView.
	for (UListDataObject_Base* TreeItem : InTreeItems)
	{
		// Ignora itens inválidos.
		if (!TreeItem) continue;
		
		// Adiciona o item válido no array final do TreeView.
		TreeItems.Add(TreeItem);
	}
	
	// Define os itens iniciais que serão exibidos pelo TreeView.
	SetListItems(TreeItems);
}

// EDITOR VALIDATION (só compila se tiver TabButtonEntryWidgetClass setado)
#if WITH_EDITOR	
void UFrontendCommonTreeView::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);
	
	// Emite erro de compilação se o DataAsset obrigatório não estiver configurado
	if (!DataListEntryMapping)
	{   
		// Emite erro de compilação informando que o Data Asset é obrigatório
		CompileLog.Error(FText::FromString(
			TEXT("The variable DataListEntryMapping hasn't valid data asset assigned ") +
			GetClass()->GetName() + 
			TEXT(" needs a valid data asset to function properly")
			));
	}
}
#endif


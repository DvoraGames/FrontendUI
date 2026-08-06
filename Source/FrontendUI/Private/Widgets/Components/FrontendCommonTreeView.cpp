// DvoraGames All Rights Reserved


#include "Widgets/Components/FrontendCommonTreeView.h"

#include "Editor/WidgetCompilerLog.h"
#include "Widgets/Options/DataAsset_DataListEntryMapping.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "Widgets/Options/DataObjects/ListDataObject_Category.h"
#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"

bool UFrontendCommonTreeView::OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem)
{
	// Rejeita itens inválidos.
	if (!FirstSelectedItem) return false;

	// Converte para o tipo esperado.
	const UListDataObject_Base* SelectedItem = Cast<UListDataObject_Base>(FirstSelectedItem);
	
	// Retorna se o item pode ser selecionado.
	return !SelectedItem->IsA<UListDataObject_Category>();
}

UUserWidget& UFrontendCommonTreeView::OnGenerateEntryWidgetInternal(UObject* Item,
	TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	// Usa o comportamento padrão durante o design time.
	if (IsDesignTime())
	{
		return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	}
	
	// Consulta o DataAsset para encontrar o widget correspondente ao tipo do DataObject
	if (const TSubclassOf<UWidget_ListEntry_Base> FoundWidgetClass = 
		DataListEntryMapping->FindEntryWidgetClassByDataObject(CastChecked<UListDataObject_Base>(Item)))
	{
		// Gera a entry tipada com o widget encontrado.
		return GenerateTypedEntry<UWidget_ListEntry_Base>(FoundWidgetClass, OwnerTable);
	}
	
	// Fallback - usa o widget padrão quando não houver mapeamento.
	return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
}

void UFrontendCommonTreeView::SetTreeViewItems(const TArray<UListDataObject_Base*>& InTreeItems)
{
	// Armazena apenas os itens válidos da árvore.
	TArray<UObject*> TreeItems;
	
	// Percorre os itens recebidos para o TreeView.
	for (UListDataObject_Base* TreeItem : InTreeItems)
	{
		// Ignora itens inválidos.
		if (!TreeItem) continue;
		
		// Adiciona o item válido no array.
		TreeItems.Add(TreeItem);
	}
	
	// Define os itens exibidos pelo TreeView.
	SetListItems(TreeItems);
}

// EDITOR VALIDATION (só compila se tiver TabButtonEntryWidgetClass setado)
#if WITH_EDITOR	
void UFrontendCommonTreeView::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);
	
	// Emite erro se o DataListEntryMapping não estiver configurado.
	if (!DataListEntryMapping)
	{   
		// Registra erro de compilação para o Blueprint.
		CompileLog.Error(FText::FromString(
			TEXT("The variable DataListEntryMapping hasn't valid data asset assigned ") +
			GetClass()->GetName() + 
			TEXT(" needs a valid data asset to function properly")
			));
	}
}
#endif


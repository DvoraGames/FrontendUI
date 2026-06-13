// DvoraGames All Rights Reserved


#include "Widgets/Components/Widget_EntryRow.h"

#include "CommonTreeView.h"
#include "Widgets/Options/DataObjects/IListDataWithChildren.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"

void UWidget_EntryRow::BuildIndent(UListDataObject_Base* InEntryDataObject)
{
	// Aborta se o DataObject for inválido.
	if (!InEntryDataObject) return;
	
	// Armazena o DataObject associado à row.
	EntryDataObject	= InEntryDataObject;
	
	// Aplica indentação apenas para itens que não estão no primeiro nível da hierarquia (root entries).
	if (EntryDataObject->GetEntryHierarchyDepth() > 0)
	{		
		// Chama a função que aplica o Indent em si na BP
		BP_SetIndent();
	}
}

void UWidget_EntryRow::RequestToggleExpansion() const
{
	// Aborta se não houver DataObject associado.
	if (!EntryDataObject) return;

	// Aborta se o item não suportar expansão.
	if (!EntryDataObject->Implements<UIListDataWithChildren>()) return;
	
	// Inverte o estado atual de expansão.
	const bool bNewExpansion = !IIListDataWithChildren::Execute_GetIsExpanded(EntryDataObject);
	
	// Salva o novo estado no DataObject.
	Cast<IIListDataWithChildren>(EntryDataObject)->SetIsExpanded(bNewExpansion);
		
	// Busca a ListEntry dona desta row para acessar o TreeView proprietário.
	if (const UWidget_ListEntry_Base* ListEntry = GetTypedOuter<UWidget_ListEntry_Base>())
	{
		// Atualiza a expansão do item no TreeView.
		CastChecked<UCommonTreeView>(ListEntry->GetOwningListView())->SetItemExpansion(EntryDataObject, bNewExpansion);
	}
}

// DvoraGames All Rights Reserved


#include "Widgets/Components/Widget_EntryRow.h"

#include "CommonTreeView.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"

void UWidget_EntryRow::BuildIndent(UListDataObject_Base* InEntryDataObject)
{
	// Interrompe se o DataObject recebido for inválido.
	if (!InEntryDataObject) return;
	
	// Armazena a referência do DataObject associado a esta row.
	EntryDataObject	= InEntryDataObject;
	
	// Aplica indentação apenas para itens que não estão no primeiro nível da hierarquia (Aba).
	if (EntryDataObject->GetHierarchyDepth() > 0)
	{		
		// Chama a função que cria o Indent em si na BP
		BP_SetIndent();
	}
}

void UWidget_EntryRow::RequestToggleExpansion()
{
	// Continua apenas se houver um DataObject associado a esta row.
	if (EntryDataObject)
	{
		// Tenta converter o item atual como uma coleção.
		if (UListDataObject_Collection* Collection = Cast<UListDataObject_Collection>(EntryDataObject))
		{
			// Inverte o estado atual de expansão da coleção.
			const bool bNewExpansion = !Collection->GetIsExpanded();
			
			// Busca a entry dona desta row para atualizar o estado visual no TreeView.
			Collection->SetIsExpanded(bNewExpansion);
			
			// Busca a ListEntry dona desta row para acessar o TreeView proprietário.
			if (const UWidget_ListEntry_Base* ListEntry = GetTypedOuter<UWidget_ListEntry_Base>())
			{
				// Atualiza o estado de expansão do item no TreeView proprietário.
				CastChecked<UCommonTreeView>(ListEntry->GetOwningListView())->SetItemExpansion(Collection, bNewExpansion);
			}
		}
	}
}

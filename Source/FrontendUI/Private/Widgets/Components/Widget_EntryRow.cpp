// DvoraGames All Rights Reserved


#include "Widgets/Components/Widget_EntryRow.h"

#include "CommonTreeView.h"
#include "Widgets/Components/ToggleActionButton.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"

void UWidget_EntryRow::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	// Configura o botão de expansão se ele estiver bindado.
	if (ToggleButton_Expansion)
	{
		// Define o tipo de ação do botão como expansão.
		ToggleButton_Expansion->SetToggleActionType(EToggleActionType::Expand);
		// Torna o botão visível mas não interativo diretamente (clique tratado pela row).
		ToggleButton_Expansion->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UWidget_EntryRow::BuildIndent(UListDataObject_Base* InEntryDataObject)
{
	// Aborta se o DataObject for inválido.
	if (!InEntryDataObject) return;
	
	// Armazena o DataObject associado à row.
	EntryDataObject	= InEntryDataObject;
	
	// Atualiza o visual do botão de expansão com o estado atual.
	RefreshExpansionButtonVisual();
	
	// Aplica indentação apenas para itens que não estão no primeiro nível da hierarquia (root entries).
	if (EntryDataObject->GetEntryHierarchyDepth() > 0)
	{		
		// Chama a função que aplica o Indent em si na BP.
		BP_SetIndent();
	}
}

void UWidget_EntryRow::RequestToggleExpansion() const
{
	// Aborta se não houver DataObject associado.
	if (!EntryDataObject) return;

	// Aborta se o item não suportar expansão.
	if (!EntryDataObject->HasAnyChildListData()) return;
	
	// Aborta se o usuário não tiver permissão para expandir manualmente.
	if (!EntryDataObject->CanUserToggleExpansion()) return;
	
	// Inverte o estado atual de expansão.
	const bool bNewExpansion = !EntryDataObject->GetbIsExpanded();
	
	// Salva o novo estado no DataObject.
	EntryDataObject->SetbIsExpanded(bNewExpansion);
		
	// Busca a ListEntry dona desta row para acessar o TreeView proprietário.
	if (const UWidget_ListEntry_Base* ListEntry = GetTypedOuter<UWidget_ListEntry_Base>())
	{
		// Atualiza a expansão do item no TreeView.
		CastChecked<UCommonTreeView>(ListEntry->GetOwningListView())->SetItemExpansion(EntryDataObject, bNewExpansion);
	}
	
	// Atualiza o visual do botão de expansão com o estado atual.
	RefreshExpansionButtonVisual();

}

bool UWidget_EntryRow::BP_CanManualExpand() const
{
	// Retorna true se houver DataObject e ele permitir expansão manual.
	return EntryDataObject && EntryDataObject->CanUserToggleExpansion();
}

void UWidget_EntryRow::RefreshExpansionButtonVisual() const
{
	// Aborta se o botão ou o DataObject não forem inválidos.
	if (!ToggleButton_Expansion || !EntryDataObject) return;

	// Sincroniza o estado visual do botão com o estado de expansão atual.
	ToggleButton_Expansion->SetIsActionActive(EntryDataObject->GetbIsExpanded());
}
// DvoraGames All Rights Reserved


#include "Widgets/Options/ListEntries/Widget_ListEntry_Category.h"
#include "CommonTreeView.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"

void UWidget_ListEntry_Category::NativeOnItemExpansionChanged(bool bIsExpanded)
{
	// Bloqueia a expansão quando a categoria não for expansível.
	if (!CachedOwningCollectionDataObject->CanUserToggleExpansion())
	{
		// Força o TreeView a manter o estado original
		CastChecked<UCommonTreeView>(GetOwningListView())->SetItemExpansion(
			CachedOwningCollectionDataObject,
			CachedOwningCollectionDataObject->GetbIsExpanded()
		);
		return;
	}
	
	// Executa o comportamento padrão, quando a categoria puder expandir.
	Super::NativeOnItemExpansionChanged(bIsExpanded);
}

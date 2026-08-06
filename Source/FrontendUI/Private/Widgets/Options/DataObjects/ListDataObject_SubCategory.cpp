// DvoraGames All Rights Reserved


#include "Widgets/Options/DataObjects/ListDataObject_SubCategory.h"


void UListDataObject_SubCategory::OnDataObjectInitialized()
{
	// Executa a inicialização da classe base.
	Super::OnDataObjectInitialized();
	
	// Define que os filhos da categoria devem ser expandidos manualmente.
	SetChildrenMode(EEntryChildrenMode::ExpandableManual);
	
	// Garante que a categoria comece recolhida.
	SetbIsExpanded(false);
}

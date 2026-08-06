// DvoraGames All Rights Reserved


#include "Widgets/Options/DataObjects/ListDataObject_Category.h"

void UListDataObject_Category::OnDataObjectInitialized()
{
	// Executa a inicialização da classe base.
	Super::OnDataObjectInitialized();
	
	// Define que os filhos da categoria devem permanecer sempre visíveis.
	SetChildrenMode(EEntryChildrenMode::AlwaysVisible);
}
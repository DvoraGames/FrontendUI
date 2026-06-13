// DvoraGames All Rights Reserved


#include "Widgets/Options/DataObjects/ListDataObject_Category.h"

void UListDataObject_Category::OnDataObjectInitialized()
{
	// Executa a inicialização da classe base.
	Super::OnDataObjectInitialized();
	
	SetIsExpandable(false);							// Define se a categoria pode ser expandida.
	SetIsExpanded(true);							// Define se a categoria inicia expandida.
}

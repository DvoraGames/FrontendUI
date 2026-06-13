// DvoraGames All Rights Reserved


#include "Widgets/Options/DataObjects/ListDataObject_TabCollection.h"

void UListDataObject_TabCollection::OnDataObjectInitialized()
{
	// Executa a inicialização da classe base.
	Super::OnDataObjectInitialized();
	
	SetIsExpandable(false);							// Define se a categoria pode ser expandida.
	SetIsExpanded(false);							// Define se a categoria inicia expandida.
}

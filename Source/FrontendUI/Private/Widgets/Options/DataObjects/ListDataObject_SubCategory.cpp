// DvoraGames All Rights Reserved


#include "Widgets/Options/DataObjects/ListDataObject_SubCategory.h"


void UListDataObject_SubCategory::OnDataObjectInitialized()
{
	Super::OnDataObjectInitialized();
	
	SetIsExpandable(true);							// Define se a categoria pode ser expandida.
	SetIsExpanded(false);							// Define se a categoria inicia expandida.
}

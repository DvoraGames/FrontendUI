// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_Category.h"
#include "ListDataObject_SubCategory.generated.h"

/**
* UListDataObject_SubCategory
*
* DataObject usado para representar subcategorias da árvore de opções.
*/
UCLASS()
class FRONTENDUI_API UListDataObject_SubCategory : public UListDataObject_Category
{
	GENERATED_BODY()
	
public:
	// ----------------------------------------------------------
	// Sobrescreve os Getters e Setters Virtuais
	// ----------------------------------------------------------
	LIST_DATA_VIRTUAL_ACCESSOR(bool, bIsExpandable); 	// Retorna e define se a categoria pode ser expandida.
	LIST_DATA_VIRTUAL_ACCESSOR(bool, bIsExpanded);		// Retorna e define se a categoria está expandida.
	
private:
	bool bIsExpandable = true;							// Define se a categoria pode ser expandida.
	bool bIsExpanded = false;							// Define se a categoria inicia expandida.
};

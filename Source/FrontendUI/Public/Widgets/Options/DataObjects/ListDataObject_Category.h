// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "ListDataObject_Category.generated.h"

/**
* UListDataObject_Category
*
* DataObject usado para representar categorias da árvore de opções.
*/
UCLASS()
class FRONTENDUI_API UListDataObject_Category : public UListDataObject_Collection
{
	GENERATED_BODY()
	
public:
	// ----------------------------------------------------------
    // Sobrescreve os Getters e Setters Virtuais
    // ----------------------------------------------------------
	LIST_DATA_VIRTUAL_ACCESSOR(bool, bIsExpandable); 	// Retorna e define se a categoria pode ser expandida.
	LIST_DATA_VIRTUAL_ACCESSOR(bool, bIsExpanded);		// Retorna e define se a categoria está expandida.
	
private:
	bool bIsExpandable = false;							// Define se a categoria pode ser expandida.
	bool bIsExpanded = true;							// Define se a categoria inicia expandida.
	
};

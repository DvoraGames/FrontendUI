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
	
protected:
	//** ----------------------------------
	//** Base
	//** ----------------------------------
	
	// Executa a inicialização específica da categoria.
	virtual void OnDataObjectInitialized() override;
};

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
	// Sobrescreve e retorna se este item pode ser selecionado na lista.
	virtual bool IsSelectable() const override { return true; }
	
protected:
	// Executa a inicialização específica da sub categoria.
	virtual void OnDataObjectInitialized() override;

};

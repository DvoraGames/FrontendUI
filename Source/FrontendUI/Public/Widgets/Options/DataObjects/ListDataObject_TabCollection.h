// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "ListDataObject_TabCollection.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintType, Hidden)
class FRONTENDUI_API UListDataObject_TabCollection : public UListDataObject_Collection
{
	GENERATED_BODY()
	
public:
	// Retorna a profundidade fixa da aba.
	virtual int32 GetTabHierarchyDepth() const { return 0; }
	
protected:
	// Executa a inicialização específica da aba.
	virtual void OnDataObjectInitialized() override;
	
};

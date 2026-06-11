// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"
#include "Widget_ListEntry_Collection.generated.h"

class UListDataObject_Collection;

/**
* UWidget_ListEntry_Collection
*
* Widget base para entries que representam collections.
* Mantém uma referência tipada para o DataObject da collection.
*/
UCLASS()
class FRONTENDUI_API UWidget_ListEntry_Collection : public UWidget_ListEntry_Base
{
	GENERATED_BODY()
	
protected:
	//Chamado logo após a Lista injetar os dados nesta entrada.
	/* Na classe base, atualiza o nome exibido e faz o bind no delegate de modificação.
	 * Subclasses devem chamar Super e implementar suas formatações específicas. */
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject) override;
	
	// DataObject tipado da collection associado à entry.
	UPROPERTY()
	UListDataObject_Collection* CachedOwningCollectionDataObject;
};

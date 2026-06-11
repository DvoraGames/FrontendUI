// DvoraGames All Rights Reserved


#include "Widgets/Options/ListEntries/Widget_ListEntry_Collection.h"

#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"


void UWidget_ListEntry_Collection::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	// Executa a lógica base da entry.
	Super::OnOwningListDataObjectSet(InOwningListDataObject);
	
	// Armazena a referência tipada da collection.
	CachedOwningCollectionDataObject = CastChecked<UListDataObject_Collection>(InOwningListDataObject);
}
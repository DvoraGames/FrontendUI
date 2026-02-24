// DvoraGames All Rights Reserved


#include "Widgets/Options/DataAsset_DataListEntryMapping.h"

#include "Widgets/Options/DataObjects/ListDataObject_Base.h"

TSubclassOf<UWidget_ListEntry_Base> UDataAsset_DataListEntryMapping::FindEntryWidgetClassByDataObject(
	const UListDataObject_Base* InDataObject) const
{
	// Verifica se o DataObject existe, caso contrario, crasha
	check(InDataObject);
	
	
	// Tenta achar o mapeamento pela classe exata; se falhar, sobe na hierarquia (SuperClass)
	for (UClass* DataObjectClass = InDataObject->GetClass(); DataObjectClass; DataObjectClass = DataObjectClass->GetSuperClass())
	{
		// Converte UClass para "TSubclassOf<UListDataObject_Base>" para consultar o TMap tipado
		if (TSubclassOf<UListDataObject_Base> ConvertDataObjectClass = TSubclassOf<UListDataObject_Base>(DataObjectClass))
		{
			// Se existe entrada para esta classe (ou alguma superclasse), retorna o widget associado
			if (DataObjectListEntryMap.Contains(ConvertDataObjectClass))
			{	
				return DataObjectListEntryMap.FindRef(ConvertDataObjectClass);
			}
		}
	}
	
	// Nenhum mapeamento encontrado: retorna classe vazia (caller deve fazer fallback)
	return TSubclassOf<UWidget_ListEntry_Base>();
}

// DvoraGames All Rights Reserved


#include "Widgets/Options/DataAsset_DataListEntryMapping.h"

#include "Widgets/Options/DataObjects/ListDataObject_Base.h"

TSubclassOf<UWidget_ListEntry_Base> UDataAsset_DataListEntryMapping::FindEntryWidgetClassByDataObject(
	const UListDataObject_Base* InDataObject) const
{
	// Garante que o DataObject é válido
	check(InDataObject);
	
	
	// Tenta achar o mapeamento pela classe exata; se falhar, sobe na hierarquia (SuperClass)
	for (UClass* DataObjectClass = InDataObject->GetClass(); DataObjectClass; DataObjectClass = DataObjectClass->GetSuperClass())
	{
		// Converte UClass para TSubclassOf tipado para consultar o TMap
		if (TSubclassOf<UListDataObject_Base> ConvertDataObjectClass = TSubclassOf<UListDataObject_Base>(DataObjectClass))
		{
			// Se o Mapeamento foi encontrado
			if (DataObjectListEntryMap.Contains(ConvertDataObjectClass))
			{	            
				// Retorna o widget associado a esta classe
				return DataObjectListEntryMap.FindRef(ConvertDataObjectClass);
			}
		}
	}
	
	// Nenhum mapeamento encontrado - retorna classe vazia, caller deve fazer fallback
	return TSubclassOf<UWidget_ListEntry_Base>();
}

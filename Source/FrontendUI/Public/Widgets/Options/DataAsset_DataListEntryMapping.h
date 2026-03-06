// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_DataListEntryMapping.generated.h"

class UWidget_ListEntry_Base;
class UListDataObject_Base;

/**
* UDataAsset_DataListEntryMapping
*
* Data Asset que mapeia tipos de DataObject ao seu Entry Widget visual correspondente.
* Usado pela FrontendCommonListView para decidir qual widget criar para cada item da lista.
*
* Se não houver mapeamento direto pela classe exata, a busca sobe na hierarquia
* de superclasses até encontrar um match — suportando herança automaticamente.
*/
UCLASS()
class FRONTENDUI_API UDataAsset_DataListEntryMapping : public UDataAsset
{
	GENERATED_BODY()
	
public:
	/* Retorna a classe do Entry Widget associada ao tipo do DataObject.
	 * Se não houver mapeamento direto, tenta as superclasses até encontrar um match. */
	TSubclassOf<UWidget_ListEntry_Base> FindEntryWidgetClassByDataObject(const UListDataObject_Base* InDataObject) const;
	
private:
	// Dicionário que associa a classe de DataObject (ex: UListDataObject_Carousel) ao widget visual que a representa.
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<UListDataObject_Base>, TSubclassOf<UWidget_ListEntry_Base>> DataObjectListEntryMap;
	
};

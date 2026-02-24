// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_DataListEntryMapping.generated.h"

class UWidget_ListEntry_Base;
class UListDataObject_Base;

/**
 * Data Asset de mapeamento entre tipos de dados da lista e seus widgets de entrada.
 * Usado pela ListView para escolher automaticamente qual widget criar para cada item.
 */
UCLASS()
class FRONTENDUI_API UDataAsset_DataListEntryMapping : public UDataAsset
{
	GENERATED_BODY()
	
public:
	/* Retorna a classe do widget de entrada associada ao tipo do DataObject.
	 * Se não houver mapeamento direto, tenta as superclasses até encontrar um match.
	 */
	TSubclassOf<UWidget_ListEntry_Base> FindEntryWidgetClassByDataObject(const UListDataObject_Base* InDataObject) const;
	
private:
	// Dicionário que associa a classe de dados (ex: UListDataObject_Carousel) ao widget visual que a representa.
	/* Usado no DataAsset */
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<UListDataObject_Base>, TSubclassOf<UWidget_ListEntry_Base>> DataObjectListEntryMap;
	
};

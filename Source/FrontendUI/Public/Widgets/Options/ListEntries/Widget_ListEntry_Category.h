// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListEntries/Widget_ListEntry_Collection.h"
#include "Widget_ListEntry_Category.generated.h"

/**
* UWidget_ListEntry_Category
*
* Widget usado para entries do tipo categoria.
* Controla o comportamento de expansão da categoria na lista.
*/
UCLASS()
class FRONTENDUI_API UWidget_ListEntry_Category : public UWidget_ListEntry_Collection
{
	GENERATED_BODY()
	
protected:
	// Atualiza o estado de expansão da categoria.
	virtual void NativeOnItemExpansionChanged(bool bIsExpanded) override;
	
};

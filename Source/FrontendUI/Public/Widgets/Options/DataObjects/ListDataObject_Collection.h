// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "ListDataObject_Collection.generated.h"

/**
* UListDataObject_Collection
*
* DataObject estrutural que representa uma aba de opções.
* Não armazena valor - apenas agrupa opções filhas (ex. Volume, SFX, Dificuldade).
*/
UCLASS(Abstract)
class FRONTENDUI_API UListDataObject_Collection : public UListDataObject_Base
{
	GENERATED_BODY()
};

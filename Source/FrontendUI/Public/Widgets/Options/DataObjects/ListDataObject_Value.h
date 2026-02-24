// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "ListDataObject_Value.generated.h"

/**
 * Classe base intermediária para opções que armazenam um valor modificável pelo usuário.
 * 
 * Atua como um agrupador lógico para diferenciar opções de interação direta (Sliders, Carousels, Toggles)
 * de opções puramente estruturais (como Collections/Abas que apenas organizam filhos).
 * Embora vazia, facilita casts, validações e iterações de salvamento (ex: salvar apenas classes que herdam desta).
 */
UCLASS(Abstract)
class FRONTENDUI_API UListDataObject_Value : public UListDataObject_Base
{
	GENERATED_BODY()
	
};

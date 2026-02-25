// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "ListDataObject_Value.generated.h"

class FOptionsDataInteractionHelper;

/**
 * Classe base intermediária para opções que armazenam e interagem com um valor (Settings).
 * 
 * Atua como um agrupador lógico para separar opções de interação direta (Sliders, Carousels, Toggles)
 * de opções puramente estruturais (como Collections/Abas que apenas agrupam filhos).
 * 
 * Centraliza os "Helpers de Reflection" (Getter e Setter dinâmicos) garantindo que toda 
 * configuração baseada em valor possa ser facilmente lida ou salva na Unreal.
 */
UCLASS(Abstract)
class FRONTENDUI_API UListDataObject_Value : public UListDataObject_Base
{
	GENERATED_BODY()
	
public:
	// Associa o Helper responsável por buscar o valor atual desta opção no backend do jogo usando Reflection.
	void SetDataDynamicGetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicGetter);
	// Associa o Helper responsável por injetar um novo valor desta opção no backend do jogo usando Reflection
	void SetDataDynamicSetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicSetter);

protected:
	// Ponteiro para o Helper que busca/lê informações nas classes de configurações do jogo.
	TSharedPtr<FOptionsDataInteractionHelper> DataDynamicGetter;
	
	// Ponteiro para o Helper que altera/escreve informações nas classes de configurações do jogo.
	TSharedPtr<FOptionsDataInteractionHelper> DataDynamicSetter;

	
};

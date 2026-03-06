// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "PropertyPathHelpers.h"

class UFrontendGameUserSettings;

/**
* FOptionsDataInteractionHelper
*
* Ponte entre a UI (DataObjects) e o backend de configurações (UFrontendGameUserSettings).
* Usa o sistema de Reflection (PropertyPathHelpers) para chamar funções Getter e Setter
* pelo nome em string — ex: "GetCurrentGameDifficulty" — sem acoplar a UI ao backend.
*
* Permite criar menus de opções totalmente dinâmicos: basta registrar o nome da função
* no construtor e o Helper faz o get/set automaticamente via Reflection.
*/
class FRONTENDUI_API FOptionsDataInteractionHelper
{
	
public:
	// Construtor - recebe o nome da função Getter ou Setter que será chamada via Reflection.
	explicit FOptionsDataInteractionHelper(const FString& InSetterOrGetterFuncPath);
	
	// Executa a função Getter via Reflection e retorna o valor atual como FString.
	FString GetValueAsString() const;
	
	// Executa a função Setter via Reflection, injetando InStringValue no backend.
	void SetValueFromString(const FString& InStringValue);
	
private:
	// Caminho/nome cacheado da propriedade/função (PropertyPath) que será executada.
	FCachedPropertyPath CachedDynamicFuncPath;
	
	/* Ponteiro fraco para o GameUserSettings - objeto alvo onde as funções são chamadas.
	 * TWeakObjectPtr evita manter o objeto vivo além do necessário. */
	TWeakObjectPtr<UFrontendGameUserSettings> CachedWeakGameUserSettings ;

};

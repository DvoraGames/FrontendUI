// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "PropertyPathHelpers.h"

class UFrontendGameUserSettings;

/**
 * Estrutura responsável por atuar como uma "ponte" entre a UI (DataObjects) e as classes
 * de configuração da Unreal (como UFrontendGameUserSettings).
 * 
 * Ele usa o sistema de Reflection (PropertyPathHelpers) para chamar as funções Get e Set
 * pelo seu nome em string (ex: "GetCurrentGameDifficulty"), permitindo criar menus de
 * opções de forma extremamente dinâmica, sem acoplar a interface ao backend.
 */
class FRONTENDUI_API FOptionsDataInteractionHelper
{
	
public:
	// Construtor do helper. InSetterOrGetterFuncPath - O nome da função a ser chamada na classe de destino.
	FOptionsDataInteractionHelper(const FString& InSetterOrGetterFuncPath);
	
	// Executa a função Getter especificada no construtor e retorna o valor dela como FString.
	FString GetValueAsString() const;
	
	// Executa a função Setter especificada no construtor, passando um novo valor para ela.
	void SetValueFromString(const FString& InStringValue);
	
private:
	// O caminho/nome cacheado da propriedade/função (PropertyPath) que será executada.
	FCachedPropertyPath CachedDynamicFuncPath;
	
	// Um ponteiro fraco para o GameUserSettings customizado do jogo.
	// É o objeto "Alvo" onde as funções Getter/Setter cadastradas serão chamadas.
	TWeakObjectPtr<UFrontendGameUserSettings> CachedWeakGameUserSettings ;

};

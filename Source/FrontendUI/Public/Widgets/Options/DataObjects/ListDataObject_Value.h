// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "ListDataObject_Value.generated.h"

class FOptionsDataInteractionHelper;

/**
* UListDataObject_Value
*
* Classe base intermediária para opções que armazenam e interagem com um valor de Settings.
* Separa opções de interação direta (Sliders, Carousels, Toggles) de opções puramente
* estruturais (Collections/Abas) que apenas agrupam filhos.
* Centraliza os Helpers de Reflection (Getter e Setter) dinâmicos.
*/
UCLASS(Abstract)
class FRONTENDUI_API UListDataObject_Value : public UListDataObject_Base
{
	GENERATED_BODY()
	
public:
	// Associa o Helper responsável por ler o valor atual desta opção no backend via Reflection.
	void SetDataDynamicGetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicGetter);
	
	// Associa o Helper responsável por injetar um novo valor desta opção no backend via Reflection.
	void SetDataDynamicSetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicSetter);
	
	// Define o valor padrão desta opção a partir de uma string.
	void SetDefaultValueFromString(const FString& InDefaultValue) { DefaultStringValue = InDefaultValue; }
	
	//~ Begin UListDataObject_Base Interface
	// Retorna true se um valor padrão foi definido para esta opção.
	virtual bool HasDefaultValue() const override { return DefaultStringValue.IsSet(); }
	//~ End UListDataObject_Base Interface

protected:
	// Retorna o valor padrão como string - válido apenas se HasDefaultValue() for true.
	FString GetDefaultValueAsString() const {return DefaultStringValue.GetValue();}
	
	// Helper que escreve informações no backend do jogo via Reflection.
	TSharedPtr<FOptionsDataInteractionHelper> DataDynamicGetter;
	
    // Helper que escreve informações no backend do jogo via Reflection.
	TSharedPtr<FOptionsDataInteractionHelper> DataDynamicSetter;
	
private:
	// Valor padrão desta opção - não definido até que SetDefaultValueFromString seja chamado.
	TOptional<FString> DefaultStringValue;

	
};

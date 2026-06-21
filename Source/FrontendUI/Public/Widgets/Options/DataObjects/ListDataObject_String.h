// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_Value.h"
#include "ListDataObject_String.generated.h"

/**
 * 
 */
UCLASS()
class FRONTENDUI_API UListDataObject_String : public UListDataObject_Value
{
	GENERATED_BODY()
	
public:
	LIST_DATA_ACCESSOR(TArray<FString>, AvailableOptionsStringArray);
	LIST_DATA_ACCESSOR(TArray<FText>, AvailableOptionsTextArray);
	LIST_DATA_ACCESSOR(FString, CurrentStringValue);
	LIST_DATA_ACCESSOR(FText, CurrentDisplayText);
	
	//~ Begin UListDataObject_Base Interface	
	// Retorna true se há valor padrão definido e o valor atual é diferente dele.
	virtual bool CanResetBackToDefaultValue() const override;
	
	// Reverte o valor atual para o padrão e notifica os widgets vinculados.
	virtual bool TryResetBackToDefaultValue() override;
	//~ End UListDataObject_Base Interface
	
	// Adiciona uma nova opção navegável ao carrossel.
	void AddDynamicOption(const FString& InStringValue, const FText& InDisplayText);
	
protected:
	//~ Begin UListDataObject_Base Interface
	// Inicializa o carrossel - define a primeira opção como selecionada por padrão.
	virtual void OnDataObjectInitialized() override;
	//~ End UListDataObject_Base Interface
	
	// Tenta atualizar o CurrentDisplayText a partir de um valor nas opções cadastradas - retorna false se não encontrado.
	bool TrySetDisplayTextFromStringValue(const FString& InStringValue);
	
private:
	
	// ----------------------------------------------------------
	// Properties
	// ----------------------------------------------------------
	
	// Valores internos/técnicos das opções - usados para salvar/carregar configs.
	TArray<FString> AvailableOptionsStringArray;
	
	// Textos de interface correspondentes aos valores internos - traduzíveis.
	TArray<FText> AvailableOptionsTextArray;
	
	// Valor interno atualmente selecionado.
	FString CurrentStringValue;
	
	// Texto de interface atualmente selecionado.
	FText CurrentDisplayText;
};

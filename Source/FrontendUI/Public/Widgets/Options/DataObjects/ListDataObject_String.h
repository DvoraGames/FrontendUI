// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_Value.h"
#include "ListDataObject_String.generated.h"

/**
* UListDataObject_String
*
* DataObject base para uma lista de opções de textos pré-definidas.
*/
UCLASS()
class FRONTENDUI_API UListDataObject_String : public UListDataObject_Value
{
	GENERATED_BODY()
	
public:
	//** ----------------------------------
	//** Base
	//** ----------------------------------
    
	/** Getters e Setters **/
	LIST_DATA_ACCESSOR(TArray<FString>, AvailableOptionsStringArray)
	LIST_DATA_ACCESSOR(TArray<FText>, AvailableOptionsTextArray)
	LIST_DATA_ACCESSOR(FString, CurrentStringValue)
	LIST_DATA_ACCESSOR(FText, CurrentDisplayText)
	LIST_DATA_ACCESSOR(bool,  bShouldActiveChildren)
	
	//~ Begin UListDataObject_Base Interface	
	// Retorna true se há valor padrão definido e o valor atual é diferente dele.
	virtual bool CanResetBackToDefaultValue() const override;
	
	// Reverte o valor atual para o padrão e notifica os widgets vinculados.
	virtual bool TryResetBackToDefaultValue() override;
	//~ End UListDataObject_Base Interface
	
	// Adiciona uma nova opção navegável ao carrossel.
	void AddDynamicOption(const FString& InStringValue, const FText& InDisplayText);
	
	// Adiciona valores que devem desativar ou colapsar os filhos.
	void AddChildrenDeactivateStringValues(const TArray<FString>& InStringArray);
	
	// Retorna a lista de valores que desativam ou colapsam os filhos.
	TArray<FString> GetCollapseOrDisableStringArray() const { return CollapseOrDisableStringArray; }
	
	// Retorna se os filhos devem ser ativados.
	bool ShouldActivateChildren() const;
	
	// Atualiza o estado de expansão dos filhos com base no valor atual.
	void RefreshChildrenExpansionState();	

protected:
	//** ----------------------------------
	//** Base
	//** ----------------------------------
	
	//~ Begin UListDataObject_Base Interface
    // Inicializa o DataObject - define o valor inicial e sincroniza o texto exibido.
	virtual void OnDataObjectInitialized() override;
	//~ End UListDataObject_Base Interface
	
	// Tenta atualizar o texto exibido a partir do valor atual.
	bool TrySetDisplayTextFromStringValue(const FString& InStringValue);

private:
	//** ----------------------------------
	//** Base - Properties
	//** ----------------------------------
	
	// Valores internos/técnicos das opções - usados para salvar/carregar configs.
	TArray<FString> AvailableOptionsStringArray;
	
	// Textos de interface correspondentes aos valores internos - traduzíveis.
	TArray<FText> AvailableOptionsTextArray;
	
	// Valor interno atualmente selecionado.
	FString CurrentStringValue;
	
	// Texto de interface atualmente selecionado.
	FText CurrentDisplayText;
	
	// Valores que desativam ou colapsam os filhos desta entry.
	TArray<FString> CollapseOrDisableStringArray;
	
	// Define se os filhos devem ser ativados.
	bool bShouldActiveChildren = false;
};
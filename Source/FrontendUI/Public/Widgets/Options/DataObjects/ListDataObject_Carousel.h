// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_Value.h"
#include "ListDataObject_Carousel.generated.h"

/**
 * DataObject para opções de menu do tipo "Carrossel".
 * Permite navegar ciclicamente (anterior/próximo) entre uma lista de opções pré-definidas.
 * Separa o valor interno (FString) do texto traduzível exibido ao jogador (FText).
 */
UCLASS()
class FRONTENDUI_API UListDataObject_Carousel : public UListDataObject_Value
{
	GENERATED_BODY()
	
public:
	// Adiciona uma nova opção navegável ao carrossel.
	void AddDynamicOption(const FString& InStringValue, const FText& InDisplayText);
	
	// Avança para a próxima opção da lista.
	void AdvanceToNextOption();
	// Retorna para a opção anterior da lista.
	void BackToPreviousOption();
	
	// Retorna a lista completa de textos exibíveis para popular o componente visual (Rotator).
	FORCEINLINE const TArray<FText>& GetAvailableOptionsTextArray() const { return AvailableOptionsTextArray; }
	// Retorna o texto da opção que está selecionada no momento.
	FORCEINLINE FText GetCurrentDisplayText() const { return CurrentDisplayText; }

protected:
	//~ Begin UListDataObject_Base Interface
	// Inicializa o carrossel, definindo a primeira opção como selecionada por padrão.
	virtual void OnDataObjectInitialized() override;
	//~ End UListDataObject_Base Interface
	
	// Tenta atualizar o CurrentDisplayText buscando o InStringValue nas opções cadastradas. 
	bool TrySetDisplayTextFromStringValue(const FString& InStringValue);

	// Valores internos/técnicos das opções (usados para salvar/carregar configs).
	TArray<FString> AvailableOptionsStringArray;
	// Textos de interface correspondentes aos valores internos (traduzíveis).
	TArray<FText> AvailableOptionsTextArray;
	
	// Valor interno atualmente selecionado.
	FString CurrentStringValue;
	// Texto de interface atualmente selecionado.
	FText CurrentDisplayText;
};

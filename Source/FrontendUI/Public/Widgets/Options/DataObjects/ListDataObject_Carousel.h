// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_Value.h"
#include "ListDataObject_Carousel.generated.h"

/**
* UListDataObject_Carousel
*
* DataObject para opções de menu do tipo Carrossel.
* Permite navegar ciclicamente (anterior/próximo) entre uma lista de opções pré-definidas.
* Separa o valor interno (FString) do texto traduzível exibido ao jogador (FText).
*/
UCLASS()
class FRONTENDUI_API UListDataObject_Carousel : public UListDataObject_Value
{
	GENERATED_BODY()
	
public:
	//~ Begin UListDataObject_Base Interface
	// Retorna true se há valor padrão definido e o valor atual é diferente dele.
	virtual bool CanResetBackToDefaultValue() const override;
	
	// Reverte o valor atual para o padrão e notifica os widgets vinculados.
	virtual bool TryResetBackToDefaultValue() override;
	//~ End UListDataObject_Base Interface
	
	// Adiciona uma nova opção navegável ao carrossel.
	void AddDynamicOption(const FString& InStringValue, const FText& InDisplayText);
	
	// Atualiza o valor selecionado a partir de um texto - chamado pelo componente visual Carousel.
	void OnCarouselInitiatedValueChange(const FText& InNewSelectedText);
	
	// Retorna a lista completa de textos exibíveis para popular o componente visual (Carousel).
	FORCEINLINE const TArray<FText>& GetAvailableOptionsTextArray() const { return AvailableOptionsTextArray; }
	
	// Retorna o texto da opção atualmente selecionada.
	FORCEINLINE FText GetCurrentDisplayText() const { return CurrentDisplayText; }

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

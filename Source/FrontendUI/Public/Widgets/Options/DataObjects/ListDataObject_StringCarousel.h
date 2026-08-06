// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ListDataObject_String.h"
#include "ListDataObject_StringCarousel.generated.h"

/**
* UListDataObject_Carousel
*
* DataObject para opções de menu do tipo Carrossel.
* Permite navegar ciclicamente (anterior/próximo) entre uma lista de opções pré-definidas.
* Separa o valor interno (FString) do texto traduzível exibido ao jogador (FText).
*/
UCLASS()
class FRONTENDUI_API UListDataObject_StringCarousel : public UListDataObject_String
{
	GENERATED_BODY()
	
public:
	//** ----------------------------------
	//** Base
	//** ----------------------------------
	
	// Atualiza o valor selecionado a partir de um texto - chamado pelo componente visual Carousel.
	void OnCarouselInitiatedValueChange(const FText& InNewSelectedText);
	
};

/**
* UListDataObject_BoolCarousel
*
* DataObject para opções booleanas exibidas como carrossel.
* Mantém dois estados fixos ("true" e "false").
* Separa o valor interno (FString) do texto traduzível exibido ao jogador (FText).
*/
UCLASS()
class FRONTENDUI_API UListDataObject_BoolCarousel : public UListDataObject_StringCarousel
{
		GENERATED_BODY()
	
public:
	//** ----------------------------------
	//** Base
	//** ----------------------------------
	
	// Sobrescreve os textos exibidos para os estados booleanos.
	void OverrideBoolDisplayTexts(const FText& InNewTrueDisplayText, const FText& InNewFalseDisplayText);
	
	// Define o valor booleano padrão da opção.
	void SetBoolDefaultValue(const bool Value);
	
protected:
	//** ----------------------------------
	//** Base
	//** ----------------------------------
	
	//~ Begin UListDataObject_Base Interface
	// Inicializa os valores booleanos antes da inicialização da classe base.
	virtual void OnDataObjectInitialized() override;
	//~ End UListDataObject_Base Interface
	
private:
    // Inicializa as opções internas do carrossel booleano.
	void TryInitBoolValues();
	
    // Valor interno usado para representar false.
	const FString FalseString = TEXT("false");
	
	// Valor interno usado para representar true.
	const FString TrueString = TEXT("true");
	
    // Texto padrão exibido para o estado false.
	FText FalseDisplayText = FText::FromString(TEXT("Off"));
	
	// Texto padrão exibido para o estado true.
	FText TrueDisplayText = FText::FromString(TEXT("On"));
};

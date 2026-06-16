// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "IListDataWithChildren.h"
#include "Widgets/Options/DataObjects/ListDataObject_Value.h"
#include "ListDataObject_Bool.generated.h"

class UFrontendCommonButtonBase;
class UFrontendCommonCarousel;

/**
 * UListDataObject_Bool
 *
 * DataObject para opções do tipo Bool dentro da ListView.
 * Armazena e gerencia o estado true/false, os textos de exibição
 * e os filhos que são exibidos quando o valor for true (caso tenha).
 */
UCLASS()
class FRONTENDUI_API UListDataObject_Bool : public UListDataObject_Value, public IIListDataWithChildren
{
	GENERATED_BODY()

public:
	//~ Begin UListDataObject_Base Interface
	// Retorna true se há valor padrão definido e o valor atual é diferente dele.
	virtual bool CanResetBackToDefaultValue() const override;
	
	// Reverte o valor atual para o padrão e notifica os widgets vinculados.
	virtual bool TryResetBackToDefaultValue() override;
	//~ End UListDataObject_Base Interface
	
	// Define o estado bool padrão inicial.
	void SetDefaultBoolValue (const bool InDefaultValue);
	
	// Define os textos exibíveis para os estados false e true.
	void SetBoolTexts(const FText& InFalseStateText, const FText& InTrueStateText);
	
	// Atualiza o valor selecionado a partir de um texto - chamado pelo componente visual Carousel.
	void OnCarouselInitiatedValueChange(const FText& InNewSelectedText);
	
	// Retorna a lista completa de textos exibíveis para popular o componente visual (Carousel).
	FORCEINLINE const TArray<FText>& GetAvailableOptionsTextArray() const { return BoolOptionsTextArray; }
	
	// Retorna o texto da opção atualmente selecionada.
	FORCEINLINE FText GetCurrentDisplayText() const { return CurrentDisplayText; }
	
	// Retorna o estado bool atualmente selecionado.
	FORCEINLINE bool GetCurrentBoolState() const { return CurrentBoolState; };
	
protected:
	//~ Begin UListDataObject_Base Interface
	// Inicializa o carrossel - define a primeira opção como selecionada por padrão.
	virtual void OnDataObjectInitialized() override;
	//~ End UListDataObject_Base Interface
	
	// Tenta atualizar o CurrentDisplayText a partir de um valor nas opções cadastradas - retorna false se não encontrado.
	bool TrySetDisplayTextFromStringValue(const FString& InStringValue);
	
private:
	// Adiciona uma opção ao arrays de string e texto.
	void AddOption(const FString& InStringValue, const FText& InDisplayText);

	// ----------------------------------------------------------
	// Properties
	// ----------------------------------------------------------
	
	// Valores internos/técnicos das opções - usados para salvar/carregar configs.
	TArray<FString> BoolOptionsStringArray;
	
	// Textos de interface correspondentes aos valores internos - traduzíveis.
	TArray<FText> BoolOptionsTextArray;
	
	// Valor interno atualmente selecionado.
	FString CurrentStringValue;
	
	// Valor bool atualmente selecionado.
	bool CurrentBoolState = false;
	
	// Texto de interface atualmente selecionado.
	FText CurrentDisplayText;
		
	// Textos de interface padrões para os estados False e True.
	FText FalseDisplayText = FText::FromString(TEXT("False"));
	FText TrueDisplayText = FText::FromString(TEXT("True"));
	
	// ----------------------------------------------------------
	// Children Manager — IIListDataWithChildren
	// ----------------------------------------------------------
public:
	//~ Start IIListDataWithChildren Interface
	// Adiciona um filho à coleção.
	virtual void AddChildListData(UListDataObject_Base* InChildListData) override;
	
	// Retorna todos os filhos da coleção.
	virtual TArray<UListDataObject_Base*> GetAllChildListData() const override;
	
	// Retorna se a coleção possui filhos.
	virtual bool HasAnyChildListData() const override;
	
	// Define se a coleção pode ser expandida.
	virtual void SetIsExpandable(bool bInIsExpandable) override { bIsExpandable = bInIsExpandable; }
	
	// Define se a coleção está expandida.
	virtual void SetIsExpanded(bool bInIsExpanded) override { bIsExpanded = bInIsExpanded; };

	// Retorna se a coleção pode ser expandida.
	virtual bool GetIsExpandable_Implementation() const override { return bIsExpandable; }

	// Retorna se a coleção está expandida.
	virtual bool GetIsExpanded_Implementation() const override { return bIsExpanded; }
	//~ End IIListDataWithChildren Interface
	
private:
	// Lista de opções pertencentes a esta aba - reconstruída em runtime, não serializada.
	UPROPERTY(Transient)
	TArray<UListDataObject_Base*> ChildListDataArray;
	
	bool bIsExpandable = false;				// Define se a coleção pode ser expandida.
	bool bIsExpanded = false; 				// Define se a coleção inicia expandida.
		
};

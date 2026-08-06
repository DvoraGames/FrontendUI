// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ListDataObject_String.h"
#include "ListDataObject_StringRotator.generated.h"

/**
* UListDataObject_StringRotator
*
* DataObject para opções de menu do tipo Rotator.
* Permite navegar entre uma lista de opções pré-definidas.
* Separa o valor interno (FString) do texto traduzível exibido ao jogador (FText).
*/
UCLASS()
class FRONTENDUI_API UListDataObject_StringRotator : public UListDataObject_String
{
	GENERATED_BODY()
	
public:
	//** ----------------------------------
	//** Base
	//** ----------------------------------
	
	// Avança para a próxima opção da lista.
	void AdvanceToNextOption();
	
	// Retorna para a opção anterior da lista.
	void BackToPreviousOption();
	
    // Atualiza o valor selecionado a partir de um texto exibido no Rotator.
	void OnRotatorInitiatedValueChange(const FText& InNewSelectedText);
	
	// Define como a navegação do Rotator deve se comportar ao chegar no limite.
	UFUNCTION(BlueprintCallable)
	void SetNavigationMode(const ERotatorLoopMode InNavigationMode);
	
	// Retorna o modo atual de navegação do Rotator.
	UFUNCTION(BlueprintPure)
	ERotatorLoopMode GetNavigationMode() const { return NavigationMode; }
	
private:
	//** ----------------------------------
	//** Base - Properties
	//** ----------------------------------
	
	// Define como a navegação do Rotator deve se comportar ao chegar no limite.
	ERotatorLoopMode NavigationMode;
};
 
UCLASS()
class FRONTENDUI_API UListDataObject_BoolRotator : public UListDataObject_StringRotator
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
	//** ----------------------------------
	//** Base - Properties
	//** ----------------------------------
	
	// Inicializa as opções internas do rotator booleano.
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
// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonNumericTextBlock.h"
#include "Widgets/Options/DataObjects/ListDataObject_Value.h"
#include "ListDataObject_Scalar.generated.h"

enum class ECommonNumericType : uint8;

/**
* UListDataObject_Scalar
*
* Objeto de dados para opções numéricas baseadas em slider.
* Responsável por mapear o valor entre a faixa de exibição e a faixa de saída,
* além de fornecer formatação numérica e suporte a reset.
*/
UCLASS()
class FRONTENDUI_API UListDataObject_Scalar : public UListDataObject_Value
{
	GENERATED_BODY()
	
public:
	//** ----------------------------------
	//** Base
	//** ----------------------------------
    
	/** Getters e Setters **/
	LIST_DATA_ACCESSOR(TRange<float>, DisplayValueRange);
	LIST_DATA_ACCESSOR(TRange<float>, OutputValueRange);
	LIST_DATA_ACCESSOR(float, SliderStepSize);
	LIST_DATA_ACCESSOR(ECommonNumericType, DisplayNumericType);
	LIST_DATA_ACCESSOR(FCommonNumberFormattingOptions, NumberFormattingOptions);
	LIST_DATA_ACCESSOR(bool, bIsLinked);
	
	//~ Begin UListDataObject_Base Interface	
	// Retorna true se há valor padrão definido e o valor atual é diferente dele.
	virtual bool CanResetBackToDefaultValue() const override;
	
	// Reverte o valor atual para o padrão e notifica os widgets vinculados.
	virtual bool TryResetBackToDefaultValue() override;
	//~ End UListDataObject_Base Interface
	
	// Retorna formatação sem casas decimais.
	static FCommonNumberFormattingOptions NoDecimal();
	
	// Retorna formatação com casas decimais.
	static FCommonNumberFormattingOptions Fraction(const int32 NumFracDigit);
	
	// Retorna o valor atual na faixa de exibição.
	float GetCurrentValue() const;
	
    // Atualiza o valor interno a partir do valor recebido pelo slider.
	void SetCurrentValueFromSlider(float InNewValue);
	
	//** ----------------------------------
	//** Linked
	//** ----------------------------------
	
	// Retorna true se houver filho Linkado
	bool HasLinkedChild() const { return !LinkedScalarChildren.IsEmpty(); }
	
	// Define as referencias dos filhos linkados via DataRegistry
	void SetLinkedScalarChildren(const TArray<UListDataObject_Scalar*>& InChildren) { LinkedScalarChildren = InChildren; }
	
protected:
	//** ----------------------------------
	//** Base
	//** ----------------------------------
	//~ Begin UListDataObject_Base Interface
	// Inicialização do DataObject_Scalar
	virtual void OnDataObjectInitialized() override;
	//~ End UListDataObject_Base Interface
	
private:
	//** ----------------------------------
	//** Base - Properties
	//** ----------------------------------
	
	// Faixa usada para exibir o valor no slider.
	TRange<float> DisplayValueRange = TRange<float>(0.f, 1.f);
	
	// Faixa real usada no backend da configuração.
	TRange<float> OutputValueRange = TRange<float>(0.f, 1.f);
	
	// Define o passo de incremento do slider.
	float SliderStepSize = .1f;
	
	// Define o tipo numérico exibido no texto.
	ECommonNumericType DisplayNumericType = ECommonNumericType::Number;
	
	// Define as opções de formatação do número exibido.
	FCommonNumberFormattingOptions NumberFormattingOptions;
	
	//** ----------------------------------
	//** Linked - Properties
	//** ----------------------------------
	
	// Indica se esta opção está com link ativo
	UPROPERTY(Transient)
	bool bIsLinked;
	
	// Armazena as referências dos filhos linkados
	UPROPERTY(Transient)
	TArray<UListDataObject_Scalar*> LinkedScalarChildren;
	
	// Executa a lógica de link entre as opções relacionadas
	virtual void OnExecuteLink() override;
};

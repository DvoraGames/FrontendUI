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
	// ----------------------------------------------------------
	// Getters e Setters — gerados via LIST_DATA_ACCESSOR
	// ----------------------------------------------------------
	LIST_DATA_ACCESSOR(TRange<float>, DisplayValueRange);
	LIST_DATA_ACCESSOR(TRange<float>, OutputValueRange);
	LIST_DATA_ACCESSOR(float, SliderStepSize);
	LIST_DATA_ACCESSOR(ECommonNumericType, DisplayNumericType);
	LIST_DATA_ACCESSOR(FCommonNumberFormattingOptions, NumberFormattingOptions);
	
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
	
	// Atualiza o valor a partir do slider.
	void SetCurrentValueFromSlider(float InNewValue);
private:
	// Converte string para float.
	static float StringToFloat(const FString &InString);
	
	// ----------------------------------------------------------
	// Properties
	// ----------------------------------------------------------
	
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
	
};

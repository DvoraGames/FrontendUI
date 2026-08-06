// DvoraGames All Rights Reserved


#include "Widgets/Options/DataObjects/ListDataObject_Scalar.h"

#include "FrontendFunctionLibrary.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"

FCommonNumberFormattingOptions UListDataObject_Scalar::NoDecimal()
{
	// Retorna formatação sem casas decimais.
	FCommonNumberFormattingOptions Options;
	Options.MaximumFractionalDigits = 0;
	return Options;
}

FCommonNumberFormattingOptions UListDataObject_Scalar::Fraction(const int32 NumFracDigit)
{
	// Retorna formatação com casas decimais baseadas no valor do parametro.
	FCommonNumberFormattingOptions Options;
	Options.MaximumFractionalDigits = NumFracDigit;
	return Options;
}

float UListDataObject_Scalar::GetCurrentValue() const
{
	// Retorna zero se o getter for inválido.
	if (!DataDynamicGetter) return 0;
	
	// Converte o valor atual interno para o valor de exibição (slider e numeric).
	return FMath::GetMappedRangeValueClamped(
	OutputValueRange, 
	DisplayValueRange, 
	StringToFloat(DataDynamicGetter->GetValueAsString())
	);
}

void UListDataObject_Scalar::SetCurrentValueFromSlider(float InNewValue)
{
    // Aborta se o setter for inválido.
	if (!DataDynamicSetter) return;
	
    // Converte o valor de exibição para o valor interno da configuração.
	const float ClampedValue = FMath::GetMappedRangeValueClamped(
		DisplayValueRange, 
		OutputValueRange,
		InNewValue
		);
	
	// Salva o valor convertido no GameUserSettings.
	DataDynamicSetter->SetValueFromString(LexToString(ClampedValue));
	
	// Notifica a alteração do valor.
	NotifyListDataModified(this);
}

bool UListDataObject_Scalar::CanResetBackToDefaultValue() const
{
	// Retorna false se não houver valor padrão ou se o getter for inválido.
	if (!HasDefaultValue() || !DataDynamicSetter) return false;
	
	// Converte o valor padrão para float.
	const float DefaultValue = StringToFloat(GetDefaultValueAsString());
	
	// Converte o valor atual interno para float.
	const float CurrentValue = StringToFloat(DataDynamicGetter->GetValueAsString());
		
	// Retorna true se o valor atual for diferente do padrão.
	return !FMath::IsNearlyEqual(CurrentValue, DefaultValue, 0.01f);
}

bool UListDataObject_Scalar::TryResetBackToDefaultValue()
{
	// Retorna false se não puder resetar ou se o setter for inválido.
	if (!CanResetBackToDefaultValue() || !DataDynamicSetter) return false;
	
	// Restaura o valor interno para o valor padrão.
	DataDynamicSetter->SetValueFromString(GetDefaultValueAsString());
		
	// Notifica a alteração do valor.
	NotifyListDataModified(this);
		
	// Retorna true se o reset foi bem sucedido
	return true;
}
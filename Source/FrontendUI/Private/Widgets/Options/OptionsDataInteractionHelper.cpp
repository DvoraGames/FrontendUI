// DvoraGames All Rights Reserved


#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "FrontendSettings/FrontendGameUserSettings.h"

FOptionsDataInteractionHelper::FOptionsDataInteractionHelper(const FString& InSetterOrGetterFuncPath) 
	: CachedDynamicFuncPath(InSetterOrGetterFuncPath) // Faz o cache da função/propriedade que será chamada posteriormente via reflection
{
	// Captura e faz cache da instância global do GameUserSettings logo na criação do Helper
	CachedWeakGameUserSettings = UFrontendGameUserSettings::Get();
}

FString FOptionsDataInteractionHelper::GetValueAsString() const
{
	FString OutStringValue;
	
	// Usa reflection para buscar e chamar a função Getter no GameUserSettings.
	// O resultado retornado pela função será armazenado na variável 'OutStringValue'
	PropertyPathHelpers::GetPropertyValueAsString(
		CachedWeakGameUserSettings.Get(), 
		CachedDynamicFuncPath, 
		OutStringValue
		);
	
	return OutStringValue;
}

void FOptionsDataInteractionHelper::SetValueFromString(const FString& InStringValue)
{
	// Usa reflection para buscar e chamar a função Setter no GameUserSettings.
	// O valor em InStringValue é injetado como parâmetro na função Set do sistema destino.
	PropertyPathHelpers::SetPropertyValueFromString(
		CachedWeakGameUserSettings.Get(), 
		CachedDynamicFuncPath, 
		InStringValue
		);
}

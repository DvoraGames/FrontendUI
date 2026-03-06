// DvoraGames All Rights Reserved


#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "FrontendSettings/FrontendGameUserSettings.h"

FOptionsDataInteractionHelper::FOptionsDataInteractionHelper(const FString& InSetterOrGetterFuncPath) 
	: CachedDynamicFuncPath(InSetterOrGetterFuncPath) // Cacheia o caminho da função para uso posterior via Reflection
{
	// Cacheia a instância global do GameUserSettings logo na criação do Helper
	CachedWeakGameUserSettings = UFrontendGameUserSettings::Get();
}

FString FOptionsDataInteractionHelper::GetValueAsString() const
{
	FString OutStringValue;
	
	// Chama a função Getter via Reflection - o resultado é armazenado em OutStringValue
	PropertyPathHelpers::GetPropertyValueAsString(
		CachedWeakGameUserSettings.Get(), 
		CachedDynamicFuncPath, 
		OutStringValue
		);
	
	// Retorna o resultado
	return OutStringValue;
}

void FOptionsDataInteractionHelper::SetValueFromString(const FString& InStringValue)
{
	// Chama a função Setter via Reflection - injeta InStringValue como parâmetro no backend
	PropertyPathHelpers::SetPropertyValueFromString(
		CachedWeakGameUserSettings.Get(), 
		CachedDynamicFuncPath, 
		InStringValue
		);
}

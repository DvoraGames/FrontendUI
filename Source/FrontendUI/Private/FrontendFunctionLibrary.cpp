// DvoraGames All Rights Reserved


#include "FrontendFunctionLibrary.h"
#include "FrontendSettings/FrontendDeveloperSettings.h"

TSoftClassPtr<UWidget_ActivatableBase> UFrontendFunctionLibrary::GetFrontendSoftWidgetClassByTag(UPARAM(meta = (Categories = "Frontend.Widget")) FGameplayTag InWidgetTag)
{
	// Acessa os valores configurados no Project Settings sem criar nova instância
	const UFrontendDeveloperSettings* FrontendDeveloperSettings = GetDefault<UFrontendDeveloperSettings>();
	
	// Garante em desenvolvimento que a tag existe no Map — crasha com log se o designer usou uma tag inválida
	checkf(FrontendDeveloperSettings->FrontendWidgetMap.
		Contains(InWidgetTag), TEXT("FrontendWidgetMap not found in FrontendDeveloperSettings"));
	
	// FindRef: retorna a soft class associada à tag e nullptr se não existir
	return FrontendDeveloperSettings->FrontendWidgetMap.FindRef(InWidgetTag);
}

TSoftObjectPtr<UTexture2D> UFrontendFunctionLibrary::GetOptionsSoftImageByTag(FGameplayTag InImageTag)
{
	// Acessa os valores configurados no Project Settings sem criar nova instância
	const UFrontendDeveloperSettings* FrontendDeveloperSettings = GetDefault<UFrontendDeveloperSettings>();
	
	// Garante em desenvolvimento que a tag existe no Map — crasha com log se o designer usou uma tag inválida
	checkf(FrontendDeveloperSettings->OptionsScreenSoftImage.
		Contains(InImageTag), TEXT("Couldn't find a image associated with tag %s"), *InImageTag.ToString());
	
	// FindRef: retorna o soft object associado à tag e nullptr se não existir
	return FrontendDeveloperSettings->OptionsScreenSoftImage.FindRef(InImageTag);
}

float HelperFunctions::StringToFloat(const FString& InString)
{
	// Inicializa o valor convertido.
	float OutConvertedValue = 0;
	
	// Converte a string recebida para float.
	LexFromString(OutConvertedValue, InString);
	
	// Retorna o valor convertido
	return OutConvertedValue;
}

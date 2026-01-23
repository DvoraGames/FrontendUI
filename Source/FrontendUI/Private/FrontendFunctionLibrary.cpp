// DvoraGames All Rights Reserved


#include "FrontendFunctionLibrary.h"
#include "FrontendSettings/FrontendDeveloperSettings.h"

TSoftClassPtr<UWidget_ActivatableBase> UFrontendFunctionLibrary::GetFrontendSoftWidgetClassByTag(UPARAM(meta = (Categories = "Frontend.Widget")) FGameplayTag InWidgetTag)
{
	// Obtém instância default das configurações (Project Settings → Frontend UI Settings)
	const UFrontendDeveloperSettings* FrontendDeveloperSettings = GetDefault<UFrontendDeveloperSettings>();
	
	// ASSERTION em desenvolvimento: garante que tag existe no FrontendWidgetMap
	// Crasha + log se designer usou tag inválida (facilita debug)
	checkf(FrontendDeveloperSettings->FrontendWidgetMap.Contains(InWidgetTag) ,TEXT("FrontendWidgetMap not found in FrontendDeveloperSettings"));
	
	// Retorna soft class associada à tag (nullptr se não existir)
	// FindRef retorna valor default (nullptr) se chave não encontrada
	return FrontendDeveloperSettings->FrontendWidgetMap.FindRef(InWidgetTag);
}
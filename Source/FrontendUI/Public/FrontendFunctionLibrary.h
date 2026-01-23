// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "FrontendFunctionLibrary.generated.h"

class UWidget_ActivatableBase;
/**
 * 
 */
UCLASS()
class FRONTENDUI_API UFrontendFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// Função utilitária que busca classe de widget por GameplayTag no FrontendUISettings
	// Retorna soft class para carregamento assíncrono - puro (sem side effects)
	UFUNCTION(
		BlueprintPure, // Função "pura" - só leitura, sem Exec pins no BP
		Category="Frontend Function Library") // Categoria no menu de contexto Blueprint
	static TSoftClassPtr<UWidget_ActivatableBase> GetFrontendSoftWidgetClassByTag(UPARAM(meta = (Categories = "Frontend.Widget"))FGameplayTag InWidgetTag); // Tag filtrada
	
};

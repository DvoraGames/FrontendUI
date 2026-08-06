// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "FrontendFunctionLibrary.generated.h"

class UWidget_ActivatableBase;

UCLASS()
class FRONTENDUI_API UFrontendFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/* Busca e retorna a Soft Class de um widget pela sua GameplayTag, mapeada no FrontendDeveloperSettings. */
	UFUNCTION(
		BlueprintPure, // Função "pura" - só leitura, sem Exec pins no BP
		Category="Frontend Function Library") // Categoria no menu de contexto Blueprint
	static TSoftClassPtr<UWidget_ActivatableBase> GetFrontendSoftWidgetClassByTag(UPARAM(meta = 
		(Categories = "Frontend.Widget"))FGameplayTag InWidgetTag); // Tag filtrada para a categoria Frontend.Widget
	
	/* Busca e retorna a Soft Image de uma opção pela sua GameplayTag, mapeada no FrontendDeveloperSettings. */
	UFUNCTION(
		BlueprintPure, // Função "pura" - só leitura, sem Exec pins no BP
		Category="Frontend Function Library") // Categoria no menu de contexto Blueprint
	static TSoftObjectPtr<UTexture2D> GetOptionsSoftImageByTag(UPARAM(meta = 
		(Categories = "Frontend.Image")) FGameplayTag InImageTag); // Tag filtrada para a categoria Frontend.Image
};

namespace HelperFunctions
{
	// Converte uma string em float, retornando 0 se a conversão falhar.
	float StringToFloat(const FString &InString);
}

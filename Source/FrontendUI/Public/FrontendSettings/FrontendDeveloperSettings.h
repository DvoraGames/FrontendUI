// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "FrontendDeveloperSettings.generated.h"

class UWidget_ActivatableBase;
/**
 * 
 */

// Configura classe como Developer Settings para Project Settings
UCLASS(
	Config = Game, // Salva configurações em DefaultGame.ini (não Engine.ini)
	defaultconfig, // Carrega valores default do .ini mesmo em editores novos
	meta = (DisplayName = "Frontend UI Settings")) // Nome mostrado na aba Project Settings
class FRONTENDUI_API UFrontendDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(
		Config, // Salva/carrega valor do DefaultGame.ini (devido ao Config=Game da UCLASS)
		EditAnywhere, // Editável em: Project Settings + instâncias da classe + Blueprints Defaults
		Category="Widget Reference", // Agrupa na aba "Widget Reference" no Project Settings
		meta=(ForceInlineRow, // TMap aparece EXPANDIDO (uma linha por entrada) ao invés de collapsed
			Categories = "Frontend.Widget")) // Filtra GameplayTags apenas da categoria "Frontend.Widget"
	TMap<FGameplayTag, TSoftClassPtr<UWidget_ActivatableBase>> FrontendWidgetMap;
};

// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "FrontendDeveloperSettings.generated.h"

class UWidget_ActivatableBase;

/**
* UFrontendDeveloperSettings
*
* Configurações do Frontend expostas no Project Settings (aba "Frontend UI Settings").
* Permite mapear GameplayTags a classes de widgets e imagens via TMap,
* centralizando referências que seriam hard references se declaradas diretamente nas classes.
*
* Os valores são salvos em DefaultGame.ini e acessados via GetDefault<UFrontendDeveloperSettings>().
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
	/* TMap de GameplayTag e Soft Class de Widget - Usado pela FrontendFunctionLibrary para buscar e carregar widgets 
	 * assincronamente por tag. */ 
	/// Configurado no Project Settings - cada entrada associa uma tag Frontend.Widget à sua classe.
	UPROPERTY(
		Config, // Salva/carrega valor do DefaultGame.ini
		EditAnywhere, // Editável em: Project Settings, instâncias da classe e Blueprints Defaults
		Category="Widget Reference", // Agrupa na aba "Widget Reference" no Project Settings
		meta=(
			ForceInlineRow, // TMap aparece expandido - uma linha por entrada ao invés de collapsed
			Categories = "Frontend.Widget")) // Filtra GameplayTags apenas da categoria "Frontend.Widget"
	TMap<FGameplayTag, TSoftClassPtr<UWidget_ActivatableBase>> FrontendWidgetMap;
	
	/* TMap de GameplayTag e Soft Object de Textura - Usado pela FrontendFunctionLibrary para buscar imagens das opções 
	 * por tag */ 
	/// Configurado no Project Settings — cada entrada associa uma tag Frontend.Image à sua textura.
	UPROPERTY(
		Config, // Salva/carrega valor do DefaultGame.ini
		EditAnywhere, // Editável em: Project Settings, instâncias da classe e Blueprints Defaults
		Category="Options Image Reference", // Agrupa na aba "Options Image Reference" no Project Settings
		meta=(
			ForceInlineRow, // TMap aparece expandido - uma linha por entrada ao invés de collapsed
			Categories = "Frontend.Image")) // Filtra GameplayTags apenas da categoria "Frontend.Image"
	TMap<FGameplayTag, TSoftObjectPtr<UTexture2D>> OptionsScreenSoftImage;
	
	// Referência soft para a SoundClass mestre, usada pela opção de volume geral.
	UPROPERTY(Config, EditAnywhere, Category = "Audio", meta = (AllowedClasses = "/Script/Engine.SoundClass"))
	FSoftObjectPath MasterSoundClass;
	
	// Referência soft para a SoundClass de música, usada pela opção de volume de música.
	UPROPERTY(Config, EditAnywhere, Category = "Audio", meta = (AllowedClasses = "/Script/Engine.SoundClass"))
	FSoftObjectPath MusicSoundClass;
	
	// Referência soft para a SoundClass de efeitos sonoros (SFX).
	UPROPERTY(Config, EditAnywhere, Category = "Audio", meta = (AllowedClasses = "/Script/Engine.SoundClass"))
	FSoftObjectPath SFXSoundClass;
	
	// Referência soft para o SoundMix padrão aplicado no jogo.
	UPROPERTY(Config, EditAnywhere, Category = "Audio", meta = (AllowedClasses = "/Script/Engine.SoundMix"))
	FSoftObjectPath DefaultSoundMix;
};

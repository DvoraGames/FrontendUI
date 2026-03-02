// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "FrontendGameUserSettings.generated.h"

/**
 * Extensão do GameUserSettings padrão da Unreal Engine.
 * Usado para adicionar, salvar e aplicar configurações customizadas do jogo (ex: Dificuldade) que persistirão no 
 * arquivo GameUserSettings.ini local do jogador.
 */
UCLASS()
class FRONTENDUI_API UFrontendGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
	
public:
	//~ Begin UMovieGraphGlobalGameOverridesNode Interface
	//  Sobrescreve ApplySettings para aplicar também configurações customizadas além das configurações padrão da Engine.
	virtual void ApplySettings(bool bCheckForCommandLineOverrides) override;
	//~ End UMovieGraphGlobalGameOverridesNode Interface

	/* Retorna a instância global e ativa das configurações do jogo.
	Facilita o acesso sem precisar chamar GEngine e fazer cast toda vez. */
	static UFrontendGameUserSettings* Get();
	
	// Retorna a instância global para uso em Blueprints.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static UFrontendGameUserSettings* GetFrontendGameUserSettings();

	/***** Gameplay Collection Tab *****/
	
	// Gameplay — Idioma ----------------------------------------------------------
	
	// Retorna a dificuldade atual do jogo salva nas configurações.
	UFUNCTION()
	FString GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }
	// Define uma nova dificuldade. Para salvar no disco, chame ApplySettings().
	UFUNCTION()
	void SetCurrentGameDifficulty(const FString& InNewDifficulty) { CurrentGameDifficulty = InNewDifficulty; }
	
	// Gameplay — Idioma ----------------------------------------------------------
	
	// Retorna o idioma atual do jogo salvo nas configurações.
	UFUNCTION()
	FString GetCurrentGameLanguage() const { return CurrentGameLanguage; }
	
	// Define um novo idioma, atualizando também a propriedade no GameUserSettings.ini.
	UFUNCTION()
	void SetCurrentGameLanguage(const FString& InNewLanguage);
	/***** Gameplay Collection Tab *****/

	
private:
	/* Armazena a dificuldade atual do jogo. A flag UPROPERTY(Config) garante que esta variável seja lida/escrita 
	automaticamente no arquivo .ini. */
	UPROPERTY(Config)
	FString CurrentGameDifficulty;
	
	/* Armazena o idioma atual do jogo. A flag UPROPERTY(Config) garante que esta variável seja lida/escrita 
	automaticamente no arquivo .ini. */
	UPROPERTY(Config)
	FString CurrentGameLanguage;
	
};

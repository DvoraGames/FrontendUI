// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "FrontendGameUserSettings.generated.h"

class UFrontendDeveloperSettings;
/**
* UFrontendGameUserSettings
*
* Extensão do GameUserSettings padrão da Unreal Engine.
* Usado para adicionar, salvar e aplicar configurações customizadas do jogo (ex: Dificuldade, Idioma)
* que persistem no arquivo GameUserSettings.ini local do jogador.
*/
UCLASS(Config = GameUserSettings, DefaultConfig)
class FRONTENDUI_API UFrontendGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
	
public:
	// Construtor - define os valores padrão.
	UFrontendGameUserSettings();
	
	//~ Begin UMovieGraphGlobalGameOverridesNode Interface
	// Aplica as configurações da Engine e as configurações customizadas.
	virtual void ApplySettings(bool bCheckForCommandLineOverrides) override;
	//~ End UMovieGraphGlobalGameOverridesNode Interface
	
	/* Retorna a instância global e ativa das configurações do jogo.
	 * Evita acessar GEngine e fazer cast manualmente. */
	static UFrontendGameUserSettings* Get();
	
	// Retorna a instância global para uso em Blueprints.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static UFrontendGameUserSettings* GetFrontendGameUserSettings();

	// ----------------------------------------------------------
	// Gameplay Collection Properties Getters/Setters
	// ----------------------------------------------------------
	
	/*** Difficulty ***/
	
	// Retorna a dificuldade atual salva nas configurações.
	UFUNCTION()
	FString GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }
	
	// Define uma nova dificuldade.
	UFUNCTION()
	void SetCurrentGameDifficulty(const FString& InNewDifficulty) { CurrentGameDifficulty = InNewDifficulty; }
	
	/*** Language ***/
	
	// Retorna o idioma atual salvo nas configurações.
	UFUNCTION()
	FString GetCurrentGameLanguage() const { return CurrentGameLanguage; }
	
	// Define um novo idioma.
	UFUNCTION()
	void SetCurrentGameLanguage(const FString& InNewLanguage);
	
	// ----------------------------------------------------------
	// Audio Collection Properties Getters/Setters
	// ----------------------------------------------------------
	
	/*** Master Volume ***/
	
	// Retorna o volume master atual salvo nas configurações.
	UFUNCTION()
	float GetCurrentMasterVolume() const { return MasterVolume; }
	
	// Define um novo volume master.
	UFUNCTION()
	void SetMasterVolume(const float InNewVolume);
	
	/*** Music Volume ***/
	
	// Retorna o volume de música atual nas configurações.
	UFUNCTION()
	float GetCurrentMusicVolume() const { return MusicVolume; }
	
	// Define um novo volume de música.
	UFUNCTION()
	void SetCurrentMusicVolume(const float InNewVolume);
	
	/*** SFX Volume ***/
	
	// Retorna o volume atual dos efeitos sonoros nas configurações.	
	UFUNCTION()
	float GetCurrentSFXVolume() const { return SFXVolume; }
	
	// Define um novo volume para os efeitos sonoros.
	UFUNCTION()
	void SetCurrentSFXVolume(const float InNewVolume);
	
	// Função helper que aplica o volume informado na SoundClass correspondente via SoundMix.
	void ApplyGameVolume(float InVolume, UObject* SoundClassObject) const;

private:
	// Cache das Developer Settings do Frontend, usadas para acessar as SoundClasses/SoundMix.
	UPROPERTY()
	const UFrontendDeveloperSettings* FrontendDeveloperSettings = GetDefault<UFrontendDeveloperSettings>();

	// ----------------------------------------------------------
	// Gameplay Collection Properties
	// ----------------------------------------------------------
	
	// Armazena a dificuldade atual - lida/escrita automaticamente no GameUserSettings.ini via Config.
	UPROPERTY(Config)
	FString CurrentGameDifficulty;
	
	// Armazena o idioma atual - lido/escrito automaticamente no GameUserSettings.ini via Config.
	UPROPERTY(Config)
	FString CurrentGameLanguage;
	
	// ----------------------------------------------------------
	// Audio Collection Properties
	// ----------------------------------------------------------
	
	// Volume master salvo no GameUserSettings.ini.
	UPROPERTY(Config)
	float MasterVolume;
	
	// Volume de música salvo no GameUserSettings.ini.
	UPROPERTY(Config)
	float MusicVolume;
	
	// Volume de efeitos salvo no GameUserSettings.ini.
	UPROPERTY(Config)
	float SFXVolume;	
};

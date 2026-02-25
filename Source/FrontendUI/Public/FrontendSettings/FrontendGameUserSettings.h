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
	/* Retorna a instância global e ativa das configurações do jogo.
	Facilita o acesso sem precisar chamar GEngine e fazer cast toda vez. */
	static UFrontendGameUserSettings* Get();
	
	/***** Gameplay Collection Tab *****/
	// Retorna a dificuldade atual do jogo salva nas configurações.
	UFUNCTION()
	FString GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }
	// Define uma nova dificuldade. Para salvar no disco, chame ApplySettings().
	UFUNCTION()
	void SetCurrentGameDifficulty(const FString& InNewDifficulty) { CurrentGameDifficulty = InNewDifficulty; }
	/***** Gameplay Collection Tab *****/

	
private:
	/* Armazena a dificuldade atual do jogo. A flag UPROPERTY(Config) garante que esta variável seja lida/escrita 
	automaticamente no arquivo .ini. */
	UPROPERTY(Config)
	FString CurrentGameDifficulty;
	
};

// DvoraGames All Rights Reserved


#include "FrontendSettings/FrontendGameUserSettings.h"

UFrontendGameUserSettings::UFrontendGameUserSettings()
: MasterVolume(1.f)
, MusicVolume(1.f)
, SFXVolume(1.f)
{
	
}

void UFrontendGameUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
	Super::ApplySettings(bCheckForCommandLineOverrides);
	
	// Reaplica o idioma salvo no .ini para garantir sincronismo após carregar as configurações
	SetCurrentGameLanguage(GetCurrentGameLanguage());
}

UFrontendGameUserSettings* UFrontendGameUserSettings::Get()
{
	if (GEngine)
	{
		// Acessa o GameUserSettings global da Engine e faz cast para a classe customizada
		return CastChecked<UFrontendGameUserSettings>(GEngine->GetGameUserSettings());
	}
	
	// Retorna nullptr se GEngine for invalida
	return nullptr;
}

UFrontendGameUserSettings* UFrontendGameUserSettings::GetFrontendGameUserSettings()
{
	// Wrapper do Get() para exposição ao Blueprint.
	return Get();
}


void UFrontendGameUserSettings::SetCurrentGameLanguage(const FString& InNewLanguage)
{
	// Atualiza a variável com o novo valor recebido - será salvo no .ini na próxima chamada de ApplySettings.
	CurrentGameLanguage = InNewLanguage;
	
	// Aplica o idioma imediatamente, trocando a cultura ativa (idioma e formatação)
	FInternationalization::Get().SetCurrentCulture(CurrentGameLanguage);
}

void UFrontendGameUserSettings::SetCurrentMasterVolume(const float InNewVolume)
{
	// Atualiza o volume master.
	MasterVolume = InNewVolume;
	
	// TODO: Aplicar o volume no jogo
}

void UFrontendGameUserSettings::SetCurrentMusicVolume(const float InNewVolume)
{
	// Atualiza o volume de música.
	MusicVolume = InNewVolume;
	
	// TODO: Aplicar o volume no jogo
}

void UFrontendGameUserSettings::SetCurrentSFXVolume(const float InNewVolume)
{
	// Atualiza o volume de efeitos sonoros.
	SFXVolume = InNewVolume;
	
	// TODO: Aplicar o volume no jogo
}

// DvoraGames All Rights Reserved


#include "FrontendSettings/FrontendGameUserSettings.h"

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

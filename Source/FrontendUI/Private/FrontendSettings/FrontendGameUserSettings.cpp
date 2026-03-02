// DvoraGames All Rights Reserved


#include "FrontendSettings/FrontendGameUserSettings.h"

void UFrontendGameUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
	Super::ApplySettings(bCheckForCommandLineOverrides);
	
	// Chama a função para atualizar o idioma com base no idioma atual do arquivo .ini
	SetCurrentGameLanguage(GetCurrentGameLanguage());
}

UFrontendGameUserSettings* UFrontendGameUserSettings::Get()
{
	// Acessa as configurações globais de usuário a partir da Engine e faz o cast para essa classe customizada
	if (GEngine)
	{
		return CastChecked<UFrontendGameUserSettings>(GEngine->GetGameUserSettings());
	}
	
	return nullptr;
}

UFrontendGameUserSettings* UFrontendGameUserSettings::GetFrontendGameUserSettings()
{
	// Retorna a referência global — wrapper para uso em Blueprint.
	return Get();
}


void UFrontendGameUserSettings::SetCurrentGameLanguage(const FString& InNewLanguage)
{
	// Atualiza a variável com o novo valor recebido. Será salvo no .ini na próxima chamada de ApplySettings.
	CurrentGameLanguage = InNewLanguage;
	
	// Aplica o idioma trocando imediatamente a cultura (Idioma e Formatação).
	FInternationalization::Get().SetCurrentCulture(CurrentGameLanguage);
}

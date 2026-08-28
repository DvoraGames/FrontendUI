// DvoraGames All Rights Reserved


#include "FrontendSettings/FrontendGameUserSettings.h"

#include "FrontendSettings/FrontendDeveloperSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"

UFrontendGameUserSettings::UFrontendGameUserSettings()
: MasterVolume(1.f)
, MusicVolume(1.f)
, SFXVolume(1.f)
{
	// Cacheia as Developer Settings do Frontend para uso nos métodos de volume.
	FrontendDeveloperSettings = GetDefault<UFrontendDeveloperSettings>();
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

void UFrontendGameUserSettings::SetMasterVolume(const float InNewVolume)
{
	// Atualiza o volume master salvo.
	MasterVolume = InNewVolume;
	
	// Aplica o novo volume na SoundClass master.
	ApplyGameVolume(MasterVolume, FrontendDeveloperSettings->MasterSoundClass.TryLoad());

}

void UFrontendGameUserSettings::SetCurrentMusicVolume(const float InNewVolume)
{
	// Atualiza o volume de música salvo.
	MusicVolume = InNewVolume;
	
	// Aplica o novo volume na SoundClass master.
	ApplyGameVolume(MusicVolume, FrontendDeveloperSettings->MusicSoundClass.TryLoad());
}

void UFrontendGameUserSettings::SetCurrentSFXVolume(const float InNewVolume)
{
	// Atualiza o volume de efeitos sonoros salvo.
	SFXVolume = InNewVolume;
	
	// Aplica o novo volume na SoundClass master.
	ApplyGameVolume(SFXVolume, FrontendDeveloperSettings->SFXSoundClass.TryLoad());
}

void UFrontendGameUserSettings::ApplyGameVolume(const float InVolume, UObject* SoundClassObject) const
{
	// Obtém o mundo de áudio ativo, se houver.
	const UWorld* InAudioWorld = GEngine ? GEngine->GetCurrentPlayWorld() : nullptr;
	
	// Aborta se não houver mundo válido ou Developer Settings configuradas.
	if (!InAudioWorld || !FrontendDeveloperSettings) return;
	
	// Faz o cast do objeto genérico para SoundClass.
	USoundClass* InSoundClass = Cast<USoundClass>(SoundClassObject);
	
	// Carrega o SoundMix padrão configurado nas Developer Settings.
	USoundMix* DefaultSoundMix = Cast<USoundMix>(FrontendDeveloperSettings->DefaultSoundMix.TryLoad());
	
	// Aborta se a SoundClass ou o SoundMix forem inválidos.
	if (!InSoundClass || !DefaultSoundMix) return;
	
	// Aplica o volume desejado como override na SoundClass, via SoundMix.
	UGameplayStatics::SetSoundMixClassOverride(
		InAudioWorld, DefaultSoundMix, InSoundClass, InVolume, 1.f, 0.2f);
	
	// Garante que o SoundMix esteja ativo para que o override tenha efeito.
	UGameplayStatics::PushSoundMixModifier(InAudioWorld, DefaultSoundMix);
}


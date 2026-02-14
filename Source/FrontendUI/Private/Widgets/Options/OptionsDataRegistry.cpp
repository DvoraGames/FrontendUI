// DvoraGames All Rights Reserved


#include "Widgets/Options/OptionsDataRegistry.h"

#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"

// Entry point - chama as 4 funções que criam as abas de configuração
void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* OwningLocalPlayer)
{
	InitGamePlayCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionTab();
	InitControlCollectionTab();
}

// Cria aba "Gameplay"
void UOptionsDataRegistry::InitGamePlayCollectionTab()
{
	// Cria instância vazia da aba
	UListDataObject_Collection* GameplayTabCollection = NewObject<UListDataObject_Collection>();
	// Define o ID da Aba
	GameplayTabCollection->SetDataID(FName("GameplayTabCollection"));
	// Define o Nome da Aba
	GameplayTabCollection->SetDataDisplayName(FText::FromString("Gameplay"));
	
	// Adiciona essa aba no CATÁLOGO CENTRAL para OptionsScreen encontrar depois
	RegisteredOptionsTabCollection.Add(GameplayTabCollection);
}

// Cria aba "Audio"
void UOptionsDataRegistry::InitAudioCollectionTab()
{
	UListDataObject_Collection* AudioTabCollection = NewObject<UListDataObject_Collection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(FText::FromString("Audio"));
	
	RegisteredOptionsTabCollection.Add(AudioTabCollection);
}

// Cria aba "Video"
void UOptionsDataRegistry::InitVideoCollectionTab()
{
	UListDataObject_Collection* VideoTabCollection = NewObject<UListDataObject_Collection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(FText::FromString("Video"));
	
	RegisteredOptionsTabCollection.Add(VideoTabCollection);
}

// Cria aba "Control"
void UOptionsDataRegistry::InitControlCollectionTab()
{
	UListDataObject_Collection* ControlTabCollection = NewObject<UListDataObject_Collection>();
	ControlTabCollection->SetDataID(FName("ControlTabCollection"));
	ControlTabCollection->SetDataDisplayName(FText::FromString("Control"));
	
	RegisteredOptionsTabCollection.Add(ControlTabCollection);
}

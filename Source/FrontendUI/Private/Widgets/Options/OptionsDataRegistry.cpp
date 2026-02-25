// DvoraGames All Rights Reserved


#include "Widgets/Options/OptionsDataRegistry.h"

#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/DataObjects/ListDataObject_Carousel.h"

// Macro para facilitar a criação e vinculação do Helper de Reflection (Getter/Setter).
/* Pega o nome da função alvo no GameUserSettings (ex: "GetCurrentGameDifficulty"), 
converte para FString e inicializa o Helper inteligente. */
#define  MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
	MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings, SetterOrGetterFuncName))

// Inicializa o catálogo criando todas as abas (e seus itens internos)
void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* OwningLocalPlayer)
{
	InitGamePlayCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionTab();
	InitControlCollectionTab();
}

TArray<UListDataObject_Base*> UOptionsDataRegistry::
GetListSourceItemBySelectedTabID(const FName InSelectedTabID) const
{
	// Encontra a coleção (aba) cujo DataID corresponde ao TabID selecionado
	UListDataObject_Collection* const* FoundTabCollectionPtr = RegisteredOptionsTabCollections.FindByPredicate(
		[InSelectedTabID](UListDataObject_Collection* AvailableTabCollection)->bool
		{
			return AvailableTabCollection->GetDataID() == InSelectedTabID;
		}
		);
	
	// Garante que o TabID é válido; se não crasha com mensagem do erro
	checkf(FoundTabCollectionPtr, TEXT("No valid Tab found under the ID %s"), *InSelectedTabID.ToString())
	
	// Retorna a lista de itens (opções) que pertencem a aba
	return (*FoundTabCollectionPtr)->GetAllChildListData();
}

// Cria aba "Gameplay"
void UOptionsDataRegistry::InitGamePlayCollectionTab()
{
	// Cria a instância da "aba" Gameplay como uma coleção (ela vai conter várias opções/entradas)
	UListDataObject_Collection* GameplayTabCollection = NewObject<UListDataObject_Collection>();
    // Identificação interna da aba (usado para seleção/busca)
	GameplayTabCollection->SetDataID(FName("GameplayTabCollection"));
    // Nome exibido no botão/rotulo da aba
	GameplayTabCollection->SetDataDisplayName(FText::FromString("Gameplay"));
	
	// Cria o Helper da opção de dificuldade, liga o DataObject da UI ao getter GetCurrentGameDifficulty do GameUserSettings via Reflection
	/** Substituido pelo macro ... para ter menos repetição e deixar a manutenção facil **/
	/*
	 *TSharedPtr<FOptionsDataInteractionHelper> ConstructedHelper = 
		MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_CHECKED(UFrontendGameUserSettings, GetCurrentGameDifficulty).ToString());
	*/
	
	
	/** Game Difficulty Option **/
	{
		// Cria a opção de dificuldade como carrossel (várias opções navegáveis)
		UListDataObject_Carousel* GameDifficulty = NewObject<UListDataObject_Carousel>();
		
		GameDifficulty->SetDataID(FName("GameDifficulty"));
		GameDifficulty->SetDataDisplayName(FText::FromString(TEXT("Difficulty")));
		
		// Registra opções dinâmicas do carrossel (valor interno + texto exibido)
		GameDifficulty->AddDynamicOption(TEXT("Easy"), FText::FromString(TEXT("Easy")));
		GameDifficulty->AddDynamicOption(TEXT("Normal"), FText::FromString(TEXT("Normal")));
		GameDifficulty->AddDynamicOption(TEXT("Hard"), FText::FromString(TEXT("Hard")));
		GameDifficulty->AddDynamicOption(TEXT("Expert"), FText::FromString(TEXT("Expert")));
		
		// Liga o DataObject da UI aos Getters/Setters do GameUserSettings via Reflection (usando a Macro)
		GameDifficulty->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameDifficulty));
		GameDifficulty->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameDifficulty));
		
		// Define que se alterar a dificuldade, o jogo deve aplicar a config imediatamente
		GameDifficulty->SetShouldApplySettingsImmediately(true);

		// Adiciona a opção dentro da aba Gameplay
		GameplayTabCollection->AddChildListData(GameDifficulty);
	}
	
	// Teste Item
	{
		UListDataObject_Carousel* TestItem = NewObject<UListDataObject_Carousel>();
		TestItem->SetDataID(FName("TestItem"));
		TestItem->SetDataDisplayName(FText::FromString("Test Item"));
		
		GameplayTabCollection->AddChildListData(TestItem);
	}
	
	// Adiciona essa aba no CATÁLOGO CENTRAL para OptionsScreen encontrar depois
	RegisteredOptionsTabCollections.Add(GameplayTabCollection);
}

// Cria aba "Audio"
void UOptionsDataRegistry::InitAudioCollectionTab()
{
	UListDataObject_Collection* AudioTabCollection = NewObject<UListDataObject_Collection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(FText::FromString("Audio"));
	
	RegisteredOptionsTabCollections.Add(AudioTabCollection);
}

// Cria aba "Video"
void UOptionsDataRegistry::InitVideoCollectionTab()
{
	UListDataObject_Collection* VideoTabCollection = NewObject<UListDataObject_Collection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(FText::FromString("Video"));
	
	RegisteredOptionsTabCollections.Add(VideoTabCollection);
}

// Cria aba "Control"
void UOptionsDataRegistry::InitControlCollectionTab()
{
	UListDataObject_Collection* ControlTabCollection = NewObject<UListDataObject_Collection>();
	ControlTabCollection->SetDataID(FName("ControlTabCollection"));
	ControlTabCollection->SetDataDisplayName(FText::FromString("Control"));
	
	RegisteredOptionsTabCollections.Add(ControlTabCollection);
}

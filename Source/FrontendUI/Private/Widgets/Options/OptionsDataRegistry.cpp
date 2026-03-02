// DvoraGames All Rights Reserved


#include "Widgets/Options/OptionsDataRegistry.h"

#include "FrontendSpacesHelper.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Internationalization/Culture.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/DataObjects/ListDataObject_Carousel.h"

using namespace FFrontendLocHelper;
using namespace FFrontendFormatCase;

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
	GameplayTabCollection->SetDataDisplayName(GetTableTextByKey("Menus.Main.Options.Gameplay"));
	
	// Cria o Helper da opção de dificuldade, liga o DataObject da UI ao getter GetCurrentGameDifficulty do GameUserSettings via Reflection
	/** Substituido pelo macro ... para ter menos repetição e deixar a manutenção facil **/
	/*
	 *TSharedPtr<FOptionsDataInteractionHelper> ConstructedHelper = 
		MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_CHECKED(UFrontendGameUserSettings, GetCurrentGameDifficulty).ToString());
	*/
	
	
	/** Game Difficulty Option **/
	{
		// Cria a opção de dificuldade do tipo carrossel (várias opções navegáveis)
		UListDataObject_Carousel* GameDifficulty = NewObject<UListDataObject_Carousel>();
		
		// Define o ID do DataObject Difficulty
		GameDifficulty->SetDataID(FName("GameDifficulty"));
		
		// Aplica o nome de exibição da Opção Difficulty usando o Helper que pega a Key da String Table
		GameDifficulty->SetDataDisplayName(GetTableTextByKey("Menus.Main.Options.Gameplay.Difficulty"));
		
		// Aplica a descrição da opção Difficulty usando o Helper que pega a Key da String Table
		GameDifficulty->SetDescriptionRichText(GetTableTextByKey("Menus.Main.Options.Gameplay.Difficulty.Description"));
		
		// Registra opções dinâmicas do carrossel (valor interno + texto exibido)
		GameDifficulty->AddDynamicOption(TEXT("Easy"), GetTableTextByKey("Menus.Main.Options.Gameplay.Difficulty.OptEasy"));
		GameDifficulty->AddDynamicOption(TEXT("Normal"), GetTableTextByKey("Menus.Main.Options.Gameplay.Difficulty.OptNormal"));
		GameDifficulty->AddDynamicOption(TEXT("Hard"), GetTableTextByKey("Menus.Main.Options.Gameplay.Difficulty.OptHard"));
		GameDifficulty->AddDynamicOption(TEXT("Expert"), GetTableTextByKey("Menus.Main.Options.Gameplay.Difficulty.OptExpert"));
		
		// Liga o DataObject da UI aos Getters/Setters do GameUserSettings via Reflection (usando a Macro)
		GameDifficulty->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameDifficulty));
		GameDifficulty->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameDifficulty));
		
		// Define que se alterar a dificuldade, o jogo deve aplicar a config imediatamente
		GameDifficulty->SetShouldApplySettingsImmediately(true);

		// Adiciona a opção dentro da aba Gameplay
		GameplayTabCollection->AddChildListData(GameDifficulty);
	}
	
	/** Game Language Option **/
	{
		// Cria a opção de idioma do tipo carrossel (várias opções navegáveis)
		UListDataObject_Carousel* GameLanguage = NewObject<UListDataObject_Carousel>();
		
		// Define o ID do DataObject Language
		GameLanguage->SetDataID(FName("GameLanguage"));
		
		// Aplica o nome de exibição da Opção Language usando o Helper que pega a Key da String Table
		GameLanguage->SetDataDisplayName(GetTableTextByKey("Menus.Main.Options.Gameplay.Language"));
		
		// Aplica a descrição da opção Language usando o Helper que pega a Key da String Table
		GameLanguage->SetDescriptionRichText(GetTableTextByKey("Menus.Main.Options.Gameplay.Language.Description"));
		
		// Array para armazenar os codigos dos idiomas disponiveis no jogo.
		// Ex: ["en", "pt-BR", "es"] — apenas os que têm arquivos ".locres" válidos.
		TArray<FString> LocalizedCodes = 
			FTextLocalizationManager::Get().GetLocalizedCultureNames(ELocalizationLoadFlags::Game);
		
		// Percorre o Array
		for (const FString& Code : LocalizedCodes)
		{
			// Obtém o objeto de cultura para acessar o nome nativo do idioma.
			TSharedPtr<FCulture> Culture = FInternationalization::Get().GetCulture(Code);
			
			// Ignora culturas inválidas.
			if (!Culture.IsValid()) continue;
			
			// Usa o nome nativo do idioma convertendo a 1 letra em maiuscula e como texto a ser exibido no carrossel
			FText NativeName = FText::FromString(UpperFirst(Culture->GetNativeName()));
			
			// Adiciona a opção ao Rotator da Entry
			GameLanguage->AddDynamicOption(Code, NativeName);
		}
		
		// Liga o DataObject da UI aos Getters/Setters do GameUserSettings via Reflection (usando a Macro)
		GameLanguage->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameLanguage));
		GameLanguage->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameLanguage));
		
		// Define que se alterar a dificuldade, o jogo deve aplicar a config imediatamente
		GameLanguage->SetShouldApplySettingsImmediately(true);
		
		// Adiciona a opção dentro da aba Gameplay
		GameplayTabCollection->AddChildListData(GameLanguage);
	}
	
	// Adiciona essa aba no CATÁLOGO CENTRAL para OptionsScreen encontrar depois
	RegisteredOptionsTabCollections.Add(GameplayTabCollection);
}

// Cria aba "Audio"
void UOptionsDataRegistry::InitAudioCollectionTab()
{
	UListDataObject_Collection* AudioTabCollection = NewObject<UListDataObject_Collection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(GetTableTextByKey("Menus.Main.Options.Audio"));
	
	RegisteredOptionsTabCollections.Add(AudioTabCollection);
}

// Cria aba "Video"
void UOptionsDataRegistry::InitVideoCollectionTab()
{
	UListDataObject_Collection* VideoTabCollection = NewObject<UListDataObject_Collection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(GetTableTextByKey("Menus.Main.Options.Video"));
	
	RegisteredOptionsTabCollections.Add(VideoTabCollection);
}

// Cria aba "Control"
void UOptionsDataRegistry::InitControlCollectionTab()
{
	UListDataObject_Collection* ControlTabCollection = NewObject<UListDataObject_Collection>();
	ControlTabCollection->SetDataID(FName("ControlTabCollection"));
	ControlTabCollection->SetDataDisplayName(GetTableTextByKey("Menus.Main.Options.Controls"));
	
	RegisteredOptionsTabCollections.Add(ControlTabCollection);
}

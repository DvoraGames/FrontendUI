// DvoraGames All Rights Reserved


#include "Widgets/Options/OptionsDataRegistry.h"

#include "FrontendNamespacesHelper.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Internationalization/Culture.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "Widgets/Options/DataObjects/ListDataObject_Category.h"
#include "Widgets/Options/DataObjects/ListDataObject_Scalar.h"
#include "Widgets/Options/DataObjects/ListDataObject_StringCarousel.h"
#include "Widgets/Options/DataObjects/ListDataObject_StringRotator.h"
#include "Widgets/Options/DataObjects/ListDataObject_SubCategory.h"
#include "Widgets/Options/DataObjects/ListDataObject_TabCollection.h"

using namespace FFrontendLocHelper;
using namespace FFrontendFormatCase;

// Macro para facilitar a criação e vinculação do Helper de Reflection (Getter/Setter).
/* Pega o nome da função alvo no GameUserSettings (ex: "GetCurrentGameDifficulty"), 
converte para FString e inicializa o Helper inteligente. */
#define  MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
	MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings, SetterOrGetterFuncName))

void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* OwningLocalPlayer)
{    
	// Inicializa todas as abas em sequência
	InitGamePlayCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionTab();
	InitControlCollectionTab();
}

TArray<UListDataObject_Base*> UOptionsDataRegistry::
GetListSourceItemBySelectedTabID(const FName InSelectedTabID) const
{
	// Busca no array a aba cujo DataID corresponde ao TabID solicitado
	UListDataObject_TabCollection* const* FoundTabCollectionPtr = RegisteredOptionsTabCollections.FindByPredicate(
		[InSelectedTabID](const UListDataObject_TabCollection* AvailableTabCollection)->bool
		{
			// Retorna true quando o DataID da aba for igual ao TabID buscado
			return AvailableTabCollection->GetDataID() == InSelectedTabID;
		}
		);
	
	// Garante que o TabID é válido - crasha com mensagem de erro se não encontrar
	checkf(FoundTabCollectionPtr, TEXT("No valid Tab found under the ID %s"), *InSelectedTabID.ToString())
	
	// Obtém a aba encontrada.
	const UListDataObject_TabCollection* FoundTabCollection = *FoundTabCollectionPtr;
	
	// Armazena todos os itens da aba.
	TArray<UListDataObject_Base*> AllChildListItems;
	
	// Percorre os itens filhos da aba.
	for (UListDataObject_Base* ChildListData : FoundTabCollection->GetAllChildListData())
	{
		// Ignora itens inválidos.
		if (!ChildListData) continue;
		
		// Adiciona o item atual ao resultado.
		AllChildListItems.Add(ChildListData);
		
		// Busca os filhos do item recursivamente.
		if (ChildListData->HasAnyChildListData())
		{
			FindChildren(ChildListData, AllChildListItems);
		}
	}

	// Retorna todos os itens encontrados.
	return AllChildListItems;
}

TArray<UListDataObject_Base*> UOptionsDataRegistry::GetTreeRootItemsBySelectedTabID(const FName InSelectedTabID) const
{	
	// Busca no array a aba cujo DataID corresponde ao TabID solicitado
	UListDataObject_TabCollection* const* FoundTabCollectionPtr = RegisteredOptionsTabCollections.FindByPredicate(
		[InSelectedTabID](const UListDataObject_TabCollection* AvailableTabCollection)->bool
		{
			// Retorna true quando o DataID da aba for igual ao TabID buscado
			return AvailableTabCollection->GetDataID() == InSelectedTabID;
		}
	);
	
	checkf(FoundTabCollectionPtr, TEXT("No valid Tab found under the ID %s"), *InSelectedTabID.ToString());

	// Obtém a aba encontrada.
	const UListDataObject_TabCollection* FoundTabCollection = *FoundTabCollectionPtr;

	// Armazena os itens raiz da aba.
	TArray<UListDataObject_Base*> RootItems;

	// Percorre os filhos diretos da aba.
	for (UListDataObject_Base* ChildListData : FoundTabCollection->GetAllChildListData())
	{
		// Ignora itens inválidos.
		if (!ChildListData) continue;
		
		// Adiciona o item raiz ao resultado.
		RootItems.Add(ChildListData);
	}

	// Retorna os itens raiz encontrados.
	return RootItems;
}

void UOptionsDataRegistry::FindChildren(UListDataObject_Base* InParentData,
                                                        TArray<UListDataObject_Base*>& OutFoundChildListData)
{
	// Aborta se o item pai for inválido ou não tiver filhos.
	if (!InParentData || !InParentData->HasAnyChildListData()) return ;
	
	// Percorre os filhos do item pai.
	for (UListDataObject_Base* Child : InParentData->GetAllChildListData())
	{
		// Ignora filhos inválidos.
		if (!Child) continue;
		
		// Adiciona o filho ao resultado.
		OutFoundChildListData.Add(Child);
		
		// Continua a busca nos próximos níveis.
		if (Child->HasAnyChildListData())
		{
			FindChildren(Child, OutFoundChildListData);
		}
	} 
}

// Cria aba "Gameplay"
void UOptionsDataRegistry::InitGamePlayCollectionTab()
{
	// Cria a instância da "aba" Gameplay como uma coleção - ela agrupa várias opções/entradas.
	UListDataObject_TabCollection* GameplayTabCollection = NewObject<UListDataObject_TabCollection>();
	
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
	
	
	/*** Game Difficulty Option ***/
	{
		// Cria a opção de dificuldade do tipo carrossel (várias opções navegáveis)
		UListDataObject_StringCarousel* GameDifficulty = NewObject<UListDataObject_StringCarousel>();
		
		// Define o ID do DataObject Difficulty
		GameDifficulty->SetDataID(FName("GameDifficulty"));
		
		// Aplica o nome de exibição da Opção Difficulty usando o Helper que pega a Key da String Table
		GameDifficulty->SetDataDisplayName(GetTableTextByKey("Menus.Main.Options.Gameplay.Difficulty"));
		
		// Aplica a descrição da opção Difficulty usando o Helper que pega a Key da String Table
		GameDifficulty->SetDescriptionRichText(GetTableTextByKey("Menus.Main.Options.Gameplay.Difficulty.Description"));
		
		// Registra opções dinâmicas do carrossel (valor interno + texto exibido)
		GameDifficulty->AddDynamicOption(TEXT("Easy"), GetTableTextByKey("Menus.Main.Options.Gameplay.Difficulty.OptEasy"));
		GameDifficulty->AddDynamicOption( TEXT("Normal"), GetTableTextByKey("Menus.Main.Options.Gameplay.Difficulty.OptNormal"));
		GameDifficulty->AddDynamicOption(TEXT("Hard"), GetTableTextByKey("Menus.Main.Options.Gameplay.Difficulty.OptHard"));
		GameDifficulty->AddDynamicOption(TEXT("Expert"), GetTableTextByKey("Menus.Main.Options.Gameplay.Difficulty.OptExpert"));
		
		// Define "Normal" como a opção padrão
		GameDifficulty->SetDefaultValueFromString(TEXT("Normal"));
		
		// Liga o DataObject da UI aos Getters/Setters do GameUserSettings via Reflection (usando a Macro)
		GameDifficulty->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameDifficulty));
		GameDifficulty->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameDifficulty));
		
		// Define que se alterar a dificuldade, o jogo deve aplicar a config imediatamente
		GameDifficulty->SetShouldApplySettingsImmediately(true);

		// Adiciona a opção dentro da aba Gameplay
		GameplayTabCollection->AddChildListData(GameDifficulty);
	}
	
	/*** Game Language Option ***/
	{
		// Cria a opção de idioma do tipo carrossel (várias opções navegáveis)
		UListDataObject_StringCarousel* GameLanguage = NewObject<UListDataObject_StringCarousel>();
		
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

		// Código de idioma padrão usado como fallback
		const FString DefaultLanguageCode = "en";
		
		// Código de idioma configurado no sistema operacional do jogador
		const FString OSLanguageCode = FInternationalization::Get().GetDefaultLanguage()->GetName();
		
		// Define o padrão: usa o idioma do OS se suportado, caso contrário cai no fallback "en"
		GameLanguage->SetDefaultValueFromString(LocalizedCodes.Contains(OSLanguageCode) ? OSLanguageCode : DefaultLanguageCode);
		
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
	UListDataObject_TabCollection* AudioTabCollection = NewObject<UListDataObject_TabCollection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(GetTableTextByKey("Menus.Main.Options.Audio"));
	
	/*** Volume Category ***/
	{
		UListDataObject_Category* VolumeCategory = NewObject<UListDataObject_Category>();
		
		VolumeCategory->SetDataID(FName("VolumeCategoryCollection"));
		VolumeCategory->SetDataDisplayName(FText::FromString(TEXT("Volume")));
		
		AudioTabCollection->AddChildListData(VolumeCategory);
		
		// Master Volume Entry
		{
			UListDataObject_Scalar* MasterVolume = NewObject<UListDataObject_Scalar>();
			MasterVolume->SetDataID(FName("MasterVolume"));
			MasterVolume->SetDataDisplayName(FText::FromString(TEXT("Master Volume")));
			MasterVolume->SetDescriptionRichText(FText::FromString(TEXT("Master Volume Description")));
			
			MasterVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			MasterVolume->SetOutputValueRange(TRange<float>(0.f, 1.f));
			MasterVolume->SetSliderStepSize(.01f);
			MasterVolume->SetDefaultValueFromString(LexToString(0.5f));
			
			MasterVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			MasterVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			
			MasterVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentMasterVolume));
			MasterVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetMasterVolume));
			
			MasterVolume->SetShouldApplySettingsImmediately(true);
			
			MasterVolume->SetToggleActionType(EToggleActionType::Mute);
			
			VolumeCategory->AddChildListData(MasterVolume);
		}
		
		// Music Volume Entry
		{
			UListDataObject_Scalar* MusicVolume = NewObject<UListDataObject_Scalar>();
			MusicVolume->SetDataID(FName("MusicVolume"));
			MusicVolume->SetDataDisplayName(FText::FromString(TEXT("Music Volume")));
			MusicVolume->SetDescriptionRichText(FText::FromString(TEXT("Music Volume Description")));
			
			MusicVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			MusicVolume->SetOutputValueRange(TRange<float>(0.f, 1.f));
			MusicVolume->SetSliderStepSize(.01f);
			
			MusicVolume->SetDefaultValueFromString(LexToString(1.f));
			
			MusicVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			MusicVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			
			MusicVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentMusicVolume));
			MusicVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentMusicVolume));
			
			MusicVolume->SetShouldApplySettingsImmediately(true);
			
			MusicVolume->SetToggleActionType(EToggleActionType::Mute);
			
			VolumeCategory->AddChildListData(MusicVolume);
		}
		
		// SFX Volume Entry
		{
			UListDataObject_Scalar* SFXVolume = NewObject<UListDataObject_Scalar>();
			SFXVolume->SetDataID(FName("SFXVolume"));
			SFXVolume->SetDataDisplayName(FText::FromString(TEXT("SFX Volume")));
			SFXVolume->SetDescriptionRichText(FText::FromString(TEXT("SFX Volume Description")));
			
			SFXVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			SFXVolume->SetOutputValueRange(TRange<float>(0.f, 1.f));
			SFXVolume->SetSliderStepSize(.01f);
			
			SFXVolume->SetDefaultValueFromString(LexToString(1.f));
			
			SFXVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			SFXVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			
			SFXVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentSFXVolume));
			SFXVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentSFXVolume));
			
			SFXVolume->SetShouldApplySettingsImmediately(true);
			
			SFXVolume->SetToggleActionType(EToggleActionType::Mute);
			
			VolumeCategory->AddChildListData(SFXVolume);
		}		
	}
	
	/*** Sound Category ***/
	{
		UListDataObject_Category* SoundCategory = NewObject<UListDataObject_Category>();
		SoundCategory->SetDataID(FName("SoundCategoryCollection"));
		SoundCategory->SetDataDisplayName(FText::FromString(TEXT("Sound")));
		
		AudioTabCollection->AddChildListData(SoundCategory);
		
		// Audio Quality Entry
		{
			UListDataObject_StringCarousel* AudioQuality = NewObject<UListDataObject_StringCarousel>();
			AudioQuality->SetDataID(FName("AudioQuality"));
			AudioQuality->SetDataDisplayName(FText::FromString(TEXT("Audio Quality")));
			
			AudioQuality->AddDynamicOption(TEXT("Mono"), FText::FromString(TEXT("Mono")));
			AudioQuality->AddDynamicOption(TEXT("Stereo"), FText::FromString(TEXT("Stereo")));
			AudioQuality->AddDynamicOption(TEXT("Surround"), FText::FromString(TEXT("Surround")));
			
			AudioQuality->SetDefaultValueFromString("Stereo");
				
			SoundCategory->AddChildListData(AudioQuality);
		}
	}
	
	RegisteredOptionsTabCollections.Add(AudioTabCollection);
}

// Cria aba "Video"
void UOptionsDataRegistry::InitVideoCollectionTab()
{
	UListDataObject_TabCollection* VideoTabCollection = NewObject<UListDataObject_TabCollection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(GetTableTextByKey("Menus.Main.Options.Video"));
	
	RegisteredOptionsTabCollections.Add(VideoTabCollection);
}

// Cria aba "Control"
void UOptionsDataRegistry::InitControlCollectionTab()
{
	UListDataObject_TabCollection* ControlTabCollection = NewObject<UListDataObject_TabCollection>();
	ControlTabCollection->SetDataID(FName("ControlTabCollection"));
	ControlTabCollection->SetDataDisplayName(GetTableTextByKey("Menus.Main.Options.Controls"));
	
	RegisteredOptionsTabCollections.Add(ControlTabCollection);
}

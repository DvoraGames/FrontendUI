// DvoraGames All Rights Reserved


#include "Widgets/Options/Widget_OptionsScreen.h"

#include "Input/CommonUIInputTypes.h"

#include "FrontendDegubHelper.h"
#include "Widgets/Components/FrontendTabListWidgetBase.h"
#include "Widgets/Options/OptionsDataRegistry.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"

void UWidget_OptionsScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	// Verifica se o ResetAction é valido, ou seja, se foi setado e não é nulo
	if (ResetAction)
	{
			// Registra binding de UI action e retorna handle
			ResetActionHandle = RegisterUIActionBinding(
		 	FBindUIActionArgs(
				ResetAction,	// Ação (InputAction)
				true,			// Mostrar na Bound Action Bar
				FSimpleDelegate::CreateUObject(this, &ThisClass::OnResetBoundActionTriggered) // Callback quando acionado
			)
			);
	}
	
	// Faz o link do callback de troca de aba
	TabListWidget_OptionsTabs->OnTabSelected.AddUniqueDynamic(this, &ThisClass::OnOptionsTabSelected);
	
}

void UWidget_OptionsScreen::NativeOnActivated()
{
	Super::NativeOnActivated();
	
	// Range-based for: Percorre todas as abas do Registry (Gameplay, Audio, Video, Control).
	// A expressão à direita do ':' executa ANTES do loop começar, não a cada iteração:
	//   1. GetOrCreateDataRegistry() retorna o Registry (cria se não existir)
	//   2. ->GetRegisteredOptionsTabCollection() retorna o array de abas
	//   3. O loop itera sobre esse array já montado
	for (UListDataObject_Collection* TabCollection : GetOrCreateDataRegistry()->GetRegisteredOptionsTabCollection())
	{
		// Pula o item do loop se a aba for nula
		if (!TabCollection)
		{
			continue;
		}
		
		// Variavel local do ID da aba
		const FName TabID = TabCollection->GetDataID();
		// Variavel local do Nome da Aba
		FText TabName = TabCollection->GetDataDisplayName();
		
		// Pula o item se já existir na lista de abas
		if (TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabID) != nullptr)
		{
			continue;
		}
		
		// Cria/Registra o Botão da aba no TabListWidget (WBP recebe e exibe)
		TabListWidget_OptionsTabs->RequestRegisterTab(TabID, TabName);
	}
}

UOptionsDataRegistry* UWidget_OptionsScreen::GetOrCreateDataRegistry()
{
	//  Lazy creation: só cria Registry na 1ª vez que NativeOnActivated roda (não cria antes desnecessariamente)
	if (!CreatedOwningDataRegistry)
	{
		// Cria a instancia do Registry "filho" dessa tela (vive enquanto OptionsScreen existir)
		CreatedOwningDataRegistry = NewObject<UOptionsDataRegistry>();
		
		// Inicializa Registry
		CreatedOwningDataRegistry->InitOptionsDataRegistry(GetOwningLocalPlayer());
	}
	
	// Crash se o Data Registry não for valido.
	checkf(CreatedOwningDataRegistry, TEXT("Data Registry for Options Screen is not valid"))
	
	// Retorna o objeto criado
	return CreatedOwningDataRegistry;
}

// Função disparada quando a Ação de Reset é disparada
void UWidget_OptionsScreen::OnResetBoundActionTriggered()
{
	Debug::Print(TEXT("Reset Settings"));
}

// Função disparada quando a Aba é Selecionada
void UWidget_OptionsScreen::OnOptionsTabSelected(FName TabID)
{
	Debug::Print(TEXT("New Tab Selected. Tab ID: ") + TabID.ToString());
}

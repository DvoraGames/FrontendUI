// DvoraGames All Rights Reserved


#include "Widgets/Options/Widget_OptionsScreen.h"

#include "Input/CommonUIInputTypes.h"

#include "FrontendDebugerHelper.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Widgets/Components/FrontendCommonListView.h"
#include "Widgets/Components/FrontendTabListWidgetBase.h"
#include "Widgets/Options/OptionsDataRegistry.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"

void UWidget_OptionsScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	// Verifica se o ResetAction é valido, ou seja, se foi configurado e não é nulo
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
	
	// Registra o handler para atualizar a lista quando o usuário trocar de aba
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
        // Ignora entradas inválidas por segurança
		if (!TabCollection)
		{
			continue;
		}
		
		// Variavel local do ID da aba
		const FName TabID = TabCollection->GetDataID();
		// Variavel local do Nome da Aba
		FText TabName = TabCollection->GetDataDisplayName();
		
        // Evita duplicar botões caso a tela seja reativada múltiplas vezes (Pop/Push do UI Stack)
		if (TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabID) != nullptr)
		{
			continue;
		}
		
        // Solicita ao TabList que crie um botão de navegação para esta aba
		TabListWidget_OptionsTabs->RequestRegisterTab(TabID, TabName);
	}
}

void UWidget_OptionsScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	
	// Ao fechar a tela de configurações, força o GameUserSettings a salvar todas as mudanças no arquivo .ini
	UFrontendGameUserSettings::Get()->ApplySettings(true);
}

UOptionsDataRegistry* UWidget_OptionsScreen::GetOrCreateDataRegistry()
{
	// Lazy Initialization: Só cria o Registry na primeira vez que ele for solicitado
	if (!CreatedOwningDataRegistry)
	{
		// Instancia o Registry. Como esta tela é o Outer, ele será destruído com ela
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
	
	// TODO: Implementar lógica real de resetar as configurações da aba atual
}

// Função disparada quando a Aba é Selecionada
void UWidget_OptionsScreen::OnOptionsTabSelected(FName TabID)
{
	// Busca no Registry todos os DataObjects (opções) que pertencem a aba recém-selecionada
	TArray<UListDataObject_Base*> FoundListSourceItems = GetOrCreateDataRegistry()->GetListSourceItemBySelectedTabID(TabID);
	
	// Alimenta e atualiza a ListView com os novos itens
	CommonListView_OptionsList->SetListItems(FoundListSourceItems);
	CommonListView_OptionsList->RequestRefresh();
	
	// Se a aba possuir itens, foca automaticamente na primeira opção (útil para navegação via Gamepad)
	if (CommonListView_OptionsList->GetListItems().Num() != 0)
	{
		CommonListView_OptionsList->NavigateToIndex(0);
		CommonListView_OptionsList->SetSelectedIndex(0);
	}
}

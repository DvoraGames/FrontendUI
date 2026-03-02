// DvoraGames All Rights Reserved


#include "Widgets/Options/Widget_OptionsScreen.h"

#include "Input/CommonUIInputTypes.h"

#include "FrontendDebugerHelper.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Widgets/Components/FrontendCommonListView.h"
#include "Widgets/Components/FrontendTabListWidgetBase.h"
#include "Widgets/Options/OptionsDataRegistry.h"
#include "Widgets/Options/Widget_OptionsDetailsView.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"

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
	
	CommonListView_OptionsList->OnItemIsHoveredChanged().AddUObject(this, &ThisClass::OnListViewItemHovered);
	CommonListView_OptionsList->OnItemSelectionChanged().AddUObject(this, &ThisClass::OnListViewItemSelected);
	
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

void UWidget_OptionsScreen::OnListViewItemHovered(UObject* InHoveredItem, bool bIsHovered)
{
	// Ignora callbacks disparados com item nulo
	if (!InHoveredItem)
	{
		return;
	}
	
	// Entry Widget correspondente ao UObject hovereado.
	UWidget_ListEntry_Base* HoveredItem = CommonListView_OptionsList->GetEntryWidgetFromItem<UWidget_ListEntry_Base>(InHoveredItem);
	
	// Verifica se o HoveredItem e valido, caso contrario, crasha
	check(HoveredItem);
	
	/* Delega o tratamento de hover ao próprio Entry Widget, passando também se o item está selecionado para que o
	Entry ajuste seu visual corretamente (hover + seleção). */
	HoveredItem->NativeOnItemHovered(bIsHovered);
	
	// Verifica se está hoverado
	if (bIsHovered)
	{
		// Atualiza as informações da DetailsView com base no Item Hovereado.
		/* As informações são preenchidas no Data Registry */
		DetailsView_ListEntryInfo->UpdateDetailsViewInfo(
			CastChecked<UListDataObject_Base>(InHoveredItem),
			TryGetEntryWidgetClassName(InHoveredItem)
			);
	}
	else
	{
		// Pega o Item atualmente selecionado, caso não tenha nenhum sendo hovereado.
		if (UListDataObject_Base* SelectedItem = CommonListView_OptionsList->GetSelectedItem<UListDataObject_Base>())
		{
			// Atualiza as informações da DetailsView com base no Item atualmente Selecionado.
			/* As informações são preenchidas no Data Registry */
			DetailsView_ListEntryInfo->UpdateDetailsViewInfo(
				SelectedItem, 
				TryGetEntryWidgetClassName(SelectedItem)
				);
		}
	}
}

void UWidget_OptionsScreen::OnListViewItemSelected(UObject* InSelectedItem)
{
	// Ignora callbacks disparados com item nulo
	if (!InSelectedItem)
	{
		return;
	}
	
	// Atualiza as informações da DetailsView com base no Item atualmente Selecionado.
	/* As informações são preenchidas no Data Registry */
	DetailsView_ListEntryInfo->UpdateDetailsViewInfo(
		CastChecked<UListDataObject_Base>(InSelectedItem), 
		TryGetEntryWidgetClassName(InSelectedItem)
		);
}

FString UWidget_OptionsScreen::TryGetEntryWidgetClassName(UObject* InOwningListItem) const
{
	// Tenta obter o Entry Widget correspondente ao DataObject fornecido.
	// GetEntryWidgetFromItem faz o mapeamento DataObject → Entry Widget gerenciado internamente pela ListView.
	if (UUserWidget* FoundEntryWidget = 
		CommonListView_OptionsList->GetEntryWidgetFromItem<UWidget_ListEntry_Base>(InOwningListItem))
	{
		// Retorna o nome da classe C++ do widget se o Entry foi encontrado.
		return FoundEntryWidget->GetClass()->GetName();
	}
	
	// Fallback: item não possui entry widget ativo no momento.
	// Pode ocorrer se o item estiver fora da área visível da ListView (reciclagem) ou se ainda não tiver sido renderizado.
	return TEXT("Entry Widget Not Valid");
}

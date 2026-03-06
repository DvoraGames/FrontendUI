// DvoraGames All Rights Reserved


#include "Widgets/Options/Widget_OptionsScreen.h"

#include "Input/CommonUIInputTypes.h"

#include "FrontendDebugerHelper.h"
#include "FrontendNamespacesHelper.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Subsystems/FrontendUISubsystem.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "Widgets/Components/FrontendCommonListView.h"
#include "Widgets/Components/FrontendTabListWidgetBase.h"
#include "Widgets/Options/OptionsDataRegistry.h"
#include "Widgets/Options/Widget_OptionsDetailsView.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"

using namespace FFrontendLocHelper;

void UWidget_OptionsScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
    // Registra o Input Action de Reset apenas se ele foi configurado no Blueprint filho.
	if (ResetAction)
	{
			// Registra o binding e salva o handle para gerenciar seu ciclo de vida.
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
	
	// Registra handlers de hover e seleção da ListView.
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
		if (!TabCollection) continue;
		
		// Variavel local do ID da aba
		const FName TabID = TabCollection->GetDataID();
		// Variavel local do Nome da Aba
		FText TabName = TabCollection->GetDataDisplayName();
		
        // Evita duplicar botões caso a tela seja reativada múltiplas vezes (Pop/Push do UI Stack)
		if (TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabID) != nullptr) continue;
		
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

UWidget* UWidget_OptionsScreen::NativeGetDesiredFocusTarget() const
{
	/* Tenta redirecionar o foco para o Entry Widget do item selecionado na ListView. Garante que o foco do CommonUI 
	sempre esteja no item correto ao navegar via Gamepad. */
	if (UObject* SelectedObject = CommonListView_OptionsList->GetSelectedItem())
	{
		if (UUserWidget* SelectedEntryWidget = CommonListView_OptionsList->GetEntryWidgetFromItem(SelectedObject))
		{
			return SelectedEntryWidget;
		}
	}
	
	return Super::NativeGetDesiredFocusTarget();
}

UOptionsDataRegistry* UWidget_OptionsScreen::GetOrCreateDataRegistry()
{
	// Lazy Initialization - só cria o Registry na primeira vez que ele for solicitado
	if (!CreatedOwningDataRegistry)
	{
		// Instancia o Registry. Como esta tela é o Outer, ele será destruído com ela
		CreatedOwningDataRegistry = NewObject<UOptionsDataRegistry>();
		
		// Inicializa Registry
		CreatedOwningDataRegistry->InitOptionsDataRegistry(GetOwningLocalPlayer());
	}
	
	// Crash se o Registry não for valido.
	checkf(CreatedOwningDataRegistry, TEXT("Data Registry for Options Screen is not valid"))
	
	// Retorna o Registry criado
	return CreatedOwningDataRegistry;
}

// Função disparada quando a Ação de Reset é disparada
void UWidget_OptionsScreen::OnResetBoundActionTriggered()
{
	// Aborta se não há nada a resetar.
	if (ResettableDataArray.IsEmpty()) return;	
	
	// Obtém a aba atualmente selecionada.
	UCommonButtonBase* SelectedTabButton = 
		TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabListWidget_OptionsTabs->GetActiveTab());
	
	// Obtém o nome da aba atualmente selecionada para exibir no modal de confirmação.
	const FString SelectedTabButtonName = 
		CastChecked<UFrontendCommonButtonBase>(SelectedTabButton)->GetButtonDisplayText().ToString();
	
	/* Obtem a mensagem da String Table e preenche o valor dinamico (entre {Valor}) com o nome da Aba e armazena o 
	resultado em uma variavel local*/
	FText ResetMessageFormatted = FText::Format(
		GetTableTextByKey("Modal.Reset.Message"), 
		FText::FromString(SelectedTabButtonName));
	
	// Exibe modal de confirmação (Yes/No) antes de resetar.
	UFrontendUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
		EConfirmScreenType::YesNo,
		GetTableTextByKey("Modal.Reset.Title"),
		ResetMessageFormatted,
			[this](EConfirmScreenButtonType ClickedButtonType)
			{
				// Aborta se o usuário cancelou o reset.
				if (ClickedButtonType != EConfirmScreenButtonType::Confirmed) return;
				
				// Define a Flag como true para evitar alterações durante o reset.
				bIsResetingData = true;
				
				// Flag para rastrear se algum DataObject falhou ao resetar.
				bool bHasDataFailedToReset = false;
				
				// Verifica cada item a lista
				for (UListDataObject_Base* DataToReset : ResettableDataArray)
				{
					// Ignora entradas inválidas por segurança.
					if (!DataToReset) continue;
					
					// Tenta dar um reset na configuração
					if (DataToReset->TryResetBackToDefaultValue())
					{
						/*** Reset bem-sucedido ***/
						
						Debug::Print(DataToReset->GetDataDisplayName().ToString() + TEXT(" was reset"));
					}
					else
					{
						/*** Reset mal-sucedido ***/
						
						// Marca falha e loga para debug.
						bHasDataFailedToReset = true;
						Debug::Print(DataToReset->GetDataDisplayName().ToString() + TEXT(" failed to reset"));
					}
				}
				
				// Se todos resetaram com sucesso
				if (!bHasDataFailedToReset)
				{
					// Limpa o array 
					ResettableDataArray.Empty();
				
					// Remove o binding de Reset da Action Bar escondendo-o
					RemoveActionBinding(ResetActionHandle);
				}
				
				// Libera a flag, delegates voltam a reagir normalmente.
				bIsResetingData = false;
			}
		);
}

void UWidget_OptionsScreen::OnOptionsTabSelected(FName TabID)
{
    // Limpa a DetailsView ao trocar de aba para não exibir dados da aba anterior.
	DetailsView_ListEntryInfo->ClearDetailsViewInfo();
	
    // Busca os DataObjects da aba selecionada no Registry.
	TArray<UListDataObject_Base*> FoundListSourceItems = GetOrCreateDataRegistry()->GetListSourceItemBySelectedTabID(TabID);
	
    // Alimenta e atualiza a ListView com os itens da aba selecionada.
	CommonListView_OptionsList->SetListItems(FoundListSourceItems);
	CommonListView_OptionsList->RequestRefresh();
	
	// Se a aba possuir itens, foca automaticamente na primeira opção (útil para navegação via Gamepad)
	if (CommonListView_OptionsList->GetListItems().Num() != 0)
	{
		CommonListView_OptionsList->NavigateToIndex(0);
		CommonListView_OptionsList->SetSelectedIndex(0);
	}
	
	// Limpa o array de resetáveis ao trocar de aba.
	ResettableDataArray.Empty();
	
	for (UListDataObject_Base* FoundListSourceItem : FoundListSourceItems)
	{
		// Ignora entradas inválidas por segurança.
		if (!FoundListSourceItem) continue;
		
		// Evita registrar o delegate de modificação mais de uma vez no mesmo DataObject.
		if (!FoundListSourceItem->OnListDataModified.IsBoundToObject(this))
		{
			// Registra o handler para reagir quando este DataObject for modificado pelo usuário.
			FoundListSourceItem->OnListDataModified.AddUObject(this,&ThisClass::OnListViewListDataModified);
		}
		
		// Verifica se este DataObject suporta reset antes de adicioná-lo ao array.
		if (FoundListSourceItem->CanResetBackToDefaultValue())
		{
			// Adiciona ao array de resetáveis.
			ResettableDataArray.AddUnique(FoundListSourceItem);
		}
	}
	
	// Verifica se o array está vazio
	if (ResettableDataArray.IsEmpty())
	{
		// Remove o Action Binding do Reset
		RemoveActionBinding(ResetActionHandle);
	}
	else
	{
		// Verifica se o array de Actions não contem o Reset
		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			// Adiciona o Action Binding do Reset
			AddActionBinding(ResetActionHandle);
		}
	}
}

void UWidget_OptionsScreen::OnListViewItemHovered(UObject* InHoveredItem, bool bIsHovered)
{
	// Ignora callbacks disparados com item nulo.	
	if (!InHoveredItem) return;
	
	// Obtém o Entry Widget correspondente ao item hovereado.
	UWidget_ListEntry_Base* HoveredItem = CommonListView_OptionsList->GetEntryWidgetFromItem<UWidget_ListEntry_Base>(InHoveredItem);
	
	// Crash se o Entry Widget for inválido.
	check(HoveredItem);
	
	// Delega o tratamento visual de hover ao próprio Entry Widget.
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
	if (!InSelectedItem) return;
	
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

void UWidget_OptionsScreen::OnListViewListDataModified(UListDataObject_Base* ModifiedData,
	EOptionsListDataModifyReason ModifiedReason)
{
	// Ignora callbacks inválidos ou disparados durante um reset em andamento.
	if (!ModifiedData || bIsResetingData) return;
	
	// Verifica se pode ser Resetado
	if (ModifiedData->CanResetBackToDefaultValue())
	{
		// Adiciona ao array o DataObject que foi modificado e pode ser resetado.
		ResettableDataArray.AddUnique(ModifiedData);
		
		
		// Verifica se o array de Actions não contem o Reset
		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			// Adiciona o Action Binding do Reset
			AddActionBinding(ResetActionHandle);
		}
	}
	else
	{
		// Verifica se o array de resetaveis ainda contem o dado modificado mesmo estando no valor padrao
		if (ResettableDataArray.Contains(ModifiedData))
		{
			// Remove o dado modificado do array
			ResettableDataArray.Remove(ModifiedData);
		}
	}
	
	// Verifica se o array está vazio
	if (ResettableDataArray.IsEmpty())
	{
		// Remove o Action Binding do Reset
		RemoveActionBinding(ResetActionHandle);
	}
}

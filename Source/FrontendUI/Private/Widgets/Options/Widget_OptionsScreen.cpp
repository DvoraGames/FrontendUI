// DvoraGames All Rights Reserved


#include "Widgets/Options/Widget_OptionsScreen.h"

#include "Input/CommonUIInputTypes.h"

#include "FrontendDebugerHelper.h"
#include "FrontendNamespacesHelper.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Subsystems/FrontendUISubsystem.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "Widgets/Components/FrontendCommonTreeView.h"
#include "Widgets/Components/FrontendTabListWidgetBase.h"
#include "Widgets/Options/OptionsDataRegistry.h"
#include "Widgets/Options/Widget_OptionsDetailsView.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/DataObjects/ListDataObject_TabCollection.h"
#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"

using namespace FFrontendLocHelper;

void UWidget_OptionsScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
    // Registra o Input Action de Reset apenas se ele foi configurado no Blueprint filho.
	if (ResetAction)
	{
		// Registra o binding da ação e armazena o handle.
		ResetActionHandle = RegisterUIActionBinding(
		FBindUIActionArgs(
			ResetAction,	// Ação (InputAction)
			true,			// Mostrar na Bound Action Bar
			FSimpleDelegate::CreateUObject(this, &ThisClass::OnResetBoundActionTriggered) // Callback quando acionado
			));
	}
	
	// Registra o handler de troca de aba.
	TabListWidget_OptionsTabs->OnTabSelected.AddUniqueDynamic(this, &ThisClass::OnOptionsTabSelected);
	
	// Verifica se a TreeView foi vinculada.
	if (CommonTreeView_OptionsList)
	{
		// Registra o callback usado para fornecer os filhos de cada item.
		CommonTreeView_OptionsList->SetOnGetItemChildren(this, &ThisClass::HandleGetItemChildren);
		
		// Registra handlers de hover e seleção da TreeView.
		CommonTreeView_OptionsList->OnItemIsHoveredChanged().AddUObject(this, &ThisClass::OnListItemHovered);
		CommonTreeView_OptionsList->OnItemSelectionChanged().AddUObject(this, &ThisClass::OnListItemSelected);
	}
}

void UWidget_OptionsScreen::NativeOnActivated()
{
	Super::NativeOnActivated();
	
	// Range-based for: Percorre todas as abas do Registry (Gameplay, Audio, Video, Control).
	// A expressão à direita do ':' executa ANTES do loop começar, não a cada iteração:
	//   1. GetOrCreateDataRegistry() retorna o Registry (cria se não existir)
	//   2. ->GetRegisteredOptionsTabCollection() retorna o array de abas
	//   3. O loop itera sobre esse array já montado
	for (UListDataObject_TabCollection* TabCollection : GetOrCreateDataRegistry()->GetRegisteredOptionsTabCollection())
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
	/* Tenta redirecionar o foco para o Entry Widget do item selecionado. Garante que o foco do CommonUI 
	sempre esteja no item correto ao navegar via Gamepad. */
	
	// Verifica se a TreeView foi vinculada.
	if (CommonTreeView_OptionsList)
	{
		// Obtém item atualmente selecionado na árvore.
		if (const UObject* SelectedObject = CommonTreeView_OptionsList->GetSelectedItem())
		{
			// Tenta obter a entry widget associada ao item selecionado.
			if (UUserWidget* SelectedEntryWidget = CommonTreeView_OptionsList->GetEntryWidgetFromItem(SelectedObject))
			{
				// Retorna a entry encontrada como alvo de foco.
				return SelectedEntryWidget;
			}
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
	
	/* Obtem a mensagem da String Table e preenche o valor dinamico {Valor} com o nome da Aba e armazena o 
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
	
	// Obtém os itens raiz (Depth 1) da aba selecionada.
	TArray<UListDataObject_Base*> FoundRootItems = GetOrCreateDataRegistry()->GetTreeRootItemsBySelectedTabID(TabID);
	
	// Verifica se a TreeView foi vinculada.
	if (CommonTreeView_OptionsList)
	{
		// Define os itens da árvore e solicita a atualização visual.
		CommonTreeView_OptionsList->SetTreeViewItems(FoundRootItems);
		CommonTreeView_OptionsList->RequestRefresh();
		
		
		// Seleciona o primeiro item selecionável disponível na árvore.
		if (CommonTreeView_OptionsList->GetListItems().Num() != 0)
		{
			// Busca o primeiro item selecional na hierarquia.
			UListDataObject_Base* FirstItemToSelect = FindFirstSelectableItem(CommonTreeView_OptionsList->GetListItems());
			
			CommonTreeView_OptionsList->SetSelectedItem(FirstItemToSelect);
			CommonTreeView_OptionsList->RequestNavigateToItem(FirstItemToSelect);
		}
	}
	
	// Limpa o array de resetáveis ao trocar de aba.
	ResettableDataArray.Empty();
		
	// Obtém todos os itens da aba para registrar delegates e estado de reset.
	TArray<UListDataObject_Base*> AllItems = GetOrCreateDataRegistry()->GetListSourceItemBySelectedTabID(TabID);
		
	for (UListDataObject_Base* Item : AllItems)
	{
		// Ignora itens inválidos.
		if (!Item) continue;
			
		// Registra esta tela para reagir a alterações do item.
		if (!Item->OnListDataModified.IsBoundToObject(this))
		{
			Item->OnListDataModified.AddUObject(this,  &ThisClass::OnListListDataModified);
		}
			
		// Adiciona o item ao array de resetáveis quando aplicável.
		if (Item->CanResetBackToDefaultValue())
		{
			ResettableDataArray.AddUnique(Item);
		}
		
		// Verifica se o item tem a Interface responsavel pelos filhos quando existir
		if (Item->HasAnyChildListData())
		{
			// Sincroniza a expansão do item com o estado do DataObject.
			CommonTreeView_OptionsList->SetItemExpansion(Item, Item->GetbIsExpanded());
		}
	}		
		
	// Remove a ação de reset quando não houver itens resetáveis.
	if (ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionHandle);
	}
	else
	{
		// Reativa a ação de reset quando necessário.
		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			AddActionBinding(ResetActionHandle);
		}
	}
}

void UWidget_OptionsScreen::OnListItemHovered(UObject* InHoveredItem, bool bIsHovered) const
{
	// Ignora callbacks com item inválido.
	if (!InHoveredItem) return;
	
	// Obtém o Entry Widget correspondente ao item hovereado.
	UWidget_ListEntry_Base* HoveredItem = CommonTreeView_OptionsList->GetEntryWidgetFromItem<UWidget_ListEntry_Base>(InHoveredItem);
	
	// Aborta se não houver entry ativa para o item.
	if (!HoveredItem) return;
	
	// Delega o estado de hover para a própria entry.
	HoveredItem->NativeOnItemHovered(bIsHovered);
	
	// Verifica se o item entrou em hover.
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
		if (UListDataObject_Base* SelectedItem = CommonTreeView_OptionsList->GetSelectedItem<UListDataObject_Base>())
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

void UWidget_OptionsScreen::OnListItemSelected(UObject* InSelectedItem) const
{
	// Ignora callbacks disparados com item nulo
	if (!InSelectedItem) return;
	
	/*** Talvez Precise Modificar pois futuras entries com filhos também poderam precisar aparecer no Detail Panel ***/
	// Ignora itens que possuem filhos.
	// if (InSelectedItem->Implements<UIListDataWithChildren>())
	// {
	// 	return;
	// }
	
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
	// GetEntryWidgetFromItem faz o mapeamento DataObject → Entry Widget gerenciado internamente pela List/Tree View.
	if (UUserWidget* FoundEntryWidget = 
		CommonTreeView_OptionsList->GetEntryWidgetFromItem<UWidget_ListEntry_Base>(InOwningListItem))
	{
		// Retorna o nome da classe C++ do widget se o Entry foi encontrado.
		return FoundEntryWidget->GetClass()->GetName();
	}
	
	// Fallback: item não possui entry widget ativo no momento.
	// Pode ocorrer se o item estiver fora da área visível da Lista (reciclagem) ou se ainda não tiver sido renderizado.
	return TEXT("Entry Widget Not Valid");
}

void UWidget_OptionsScreen::OnListListDataModified(UListDataObject_Base* ModifiedData,
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

void UWidget_OptionsScreen::HandleGetItemChildren(UObject* InItem, TArray<UObject*>& OutChildren)
{
	// Tenta obter o DataObject associado ao item da árvore.
	UListDataObject_Base* FoundData = Cast<UListDataObject_Base>(InItem);
	
	// Aborta se o item for inválido ou não possuir filhos.
	if (!FoundData || !FoundData->HasAnyChildListData()) return;
	
	if (!FoundData->HasAnyChildListData()) return;
	
	// Adiciona os filhos válidos no array de saída do TreeView.
	for (UListDataObject_Base* ChildData : FoundData->GetAllChildListData())
	{
		// Ignora filhos inválidos.
		if (!ChildData) continue;
		
		// Define o pai hierárquico do filho quando necessário.
		if (!ChildData->GetParentData())
		{
			ChildData->SetParentData(FoundData);
		}
		
		// Entrega o filho ao TreeView.
		OutChildren.Add(ChildData);
	}
}

UListDataObject_Base* UWidget_OptionsScreen::FindFirstSelectableItem(const TArray<UObject*>& Items)
{
	// Percorre cada item do array
	for (UObject* Item : Items)
	{
		// Tenta converter o item em um ListDataObject_Base
		UListDataObject_Base* DataObject = Cast<UListDataObject_Base>(Item);
		
		// Ignora se o DataObject for inválido.
		if (!DataObject) continue;
		
		// Retorna o DataObject imediatamente se o mesmo for selecionável.
		if (!DataObject->UsesAlwaysVisibleChildren()) return DataObject;
		
		// Busca nos filhos se o item for uma coleção.
		if (DataObject->HasAnyChildListData())
		{
			// Obtém os filhos do item atual.
			TArray<UListDataObject_Base*> Children = DataObject->GetAllChildListData();
			
			// Converte para TArray<UObject*> para compatibilidade com a função.
			TArray<UObject*> ChildObjects(Children);
			
			// Retorna o primeiro selecionável encontrado nos filhos.
			if (UListDataObject_Base* Found = FindFirstSelectableItem(ChildObjects))
			{
				return Found;
			}
		}
	}

	// Retorna null se nenhum item selecionável for encontrado.
	return nullptr;
}
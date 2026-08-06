// DvoraGames All Rights Reserved


#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"

#include "CommonActionWidget.h"
#include "CommonInputSubsystem.h"
#include "CommonTextBlock.h"
#include "CommonTreeView.h"
#include "Components/ListView.h"
#include "Input/CommonUIInputTypes.h"
#include "Widgets/Components/ToggleActionButton.h"
#include "Widgets/Components/Widget_EntryRow.h"
#include "Widgets/Components/Widget_ToggleAction.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "Widgets/Options/DataObjects/ListDataObject_Value.h"

void UWidget_ListEntry_Base::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	// Vincula o clique do toggle action, se o widget estiver presente.
	if (WBP_ToggleAction)
	{
		WBP_ToggleAction->GetToggleButton()->OnClicked().AddUObject(this, &ThisClass::OnToggleActionClicked);
	}
}

void UWidget_ListEntry_Base::NativeOnItemHovered(bool bIsHovered)
{
	// Repassa o estado de hover da entry para o Blueprint.
	BP_OnItemHovered(bIsHovered, IsListItemSelected());
	
	// Repassa o hover também para a row auxiliar, se existir.
	if (WBP_Entry_RowLayout)
	{
		WBP_Entry_RowLayout->BP_OnOwningEntryHovered(bIsHovered, IsListItemSelected());
	}
}

void UWidget_ListEntry_Base::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Executa a lógica padrão da interface base.
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	
	// Garante que a entry reciclada volte visível.
	SetVisibility(ESlateVisibility::Visible);
	
	// Converte o item genérico da ListView para o DataObject esperado e inicia o setup visual/lógico da entry
	OnOwningListDataObjectSet(CastChecked<UListDataObject_Base>(ListItemObject));
}

void UWidget_ListEntry_Base::NativeOnEntryReleased()
{    
	// Executa a lógica padrão da interface base.
	IUserObjectListEntry::NativeOnEntryReleased();
	
	// Remove o estado de hover da linha antes da reciclagem.
	NativeOnItemHovered(false);
	
	// Atualiza o toggle action antes da reciclagem da entry.
	RefreshToggleActionPresentation();
	
	// Remove o binding de input, já que a entry está sendo liberada.
	UnregisterToggleActionBinding();
}

void UWidget_ListEntry_Base::NativeOnItemSelectionChanged(const bool bIsSelected)
{
	// Executa a lógica padrão da interface base.
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);
	
	// Notifica a row auxiliar sobre a mudança de seleção.
	if (WBP_Entry_RowLayout)
	{
		WBP_Entry_RowLayout->BP_OnOwningEntrySelected(bIsSelected);
	}
	
	// Registra o input de toggle apenas quando a entry estiver selecionada.
	if (bIsSelected)
	{
		RegisterToggleActionBinding();
	}
	else
	{
		UnregisterToggleActionBinding();
	}
	
	// Atualiza a apresentação do toggle action com o novo estado de seleção.
	RefreshToggleActionPresentation();
}

FReply UWidget_ListEntry_Base::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
    // Obtém e verifica se o subsistema de input é valido - usado para identificar o dispositivo atual
	if (const UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem())
	{
		// Consulta o subsistema de input para saber se o foco veio de navegação por Gamepad
		if (CommonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
		{
			// "Pergunta" ao Blueprint qual widget interno desta entry deve receber o foco
			if (const UWidget* WidgetToFocus = BP_GetWidgetToFocusForGamepad())
			{
				// Obtém o widget Slate já construído para conseguir aplicar o foco de fato
				if (const TSharedPtr<SWidget> SlateWidgetToFocus = WidgetToFocus->GetCachedWidget())
				{
					RefreshToggleActionPresentation();
					
					// Intercepta o foco desta entry e redireciona para o widget interno (Rotator, etc) retornado pelo BP
					return FReply::Handled().SetUserFocus(SlateWidgetToFocus.ToSharedRef());
				}
			}
		}
	}
	
	// Se nenhuma regra customizada for aplicada, usa o comportamento padrão da classe base
	return Super::NativeOnFocusReceived(InGeometry,InFocusEvent);
}

void UWidget_ListEntry_Base::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	// Armazena o DataObject associado à entry.
	OwningListDataObject = InOwningListDataObject;
	
	// Verifica se o widget de texto foi vinculado no Blueprint
	if (CommonText_SettingDisplayName)
	{    
		// Atualiza o texto da entry com o nome da configuração (ex: Difficulty, Language, Volume)
		CommonText_SettingDisplayName->SetText(InOwningListDataObject->GetDataDisplayName());
	}
	
	// Evita registrar o mesmo bind mais de uma vez.
	if (!InOwningListDataObject->OnListDataModified.IsBoundToObject(this))
	{
		// Faz esta entry escutar mudanças do seu DataObject para que subclasses possam ressincronizar a UI
		InOwningListDataObject->OnListDataModified.AddUObject(this, &ThisClass::OnOwningListDataObjectModified);
	}
	
	// Verifica se a row auxiliar foi vinculada no Blueprint.
	if (WBP_Entry_RowLayout)
	{
		// Monta a indentação com base na hierarquia.
		WBP_Entry_RowLayout->BuildIndent(InOwningListDataObject);
		
		// Só permite exibir estado de expansão se a entry suportar toggle manual ou auto expansão.
		const bool bCanShowExpansionState = 
			OwningListDataObject->CanUserToggleExpansion() ||
			OwningListDataObject->UsesAutoExpansion();
			
		
		// Sincroniza a row com o estado atual da coleção.
		WBP_Entry_RowLayout->BP_OnItemExpansionChanged(
			bCanShowExpansionState, 
			OwningListDataObject->GetbIsExpanded()
			);
	}
	
	// Atualiza o toggle action com os dados recém setados.
	RefreshToggleActionPresentation();
}

void UWidget_ListEntry_Base::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData,
	EOptionsListDataModifyReason ModifyReason)
{
	// Intencionalmente vazio na base - subclasses sobrescrevem para atualizar seus controles visuais
	
	// Ainda assim, mantém o toggle action sincronizado após qualquer modificação.
	RefreshToggleActionPresentation();
}

void UWidget_ListEntry_Base::NativeOnItemExpansionChanged(const bool bIsExpanded)
{
	// Executa a lógica padrão da interface base.
	IUserObjectListEntry::NativeOnItemExpansionChanged(bIsExpanded);
	
	// Se o usuário pode expandir manualmente, salva o novo estado no DataObject.
	if (OwningListDataObject->CanUserToggleExpansion())
	{
		OwningListDataObject->SetbIsExpanded(bIsExpanded);
	}
	else
	{
		// Caso contrário, força a TreeView a respeitar o estado atual do DataObject (ex: auto expansão).
		CastChecked<UCommonTreeView>(GetOwningListView())->SetItemExpansion(
			OwningListDataObject, 
			OwningListDataObject->GetbIsExpanded());

	}
	
	// Verifica se a WBP_EntryRow e OwningListDataObject são validos
	if (WBP_Entry_RowLayout && OwningListDataObject)
	{
		// Só permite exibir estado de expansão se a entry suportar toggle manual ou auto expansão.
		const bool bExpansionState = 
			OwningListDataObject->CanUserToggleExpansion() ||
			OwningListDataObject->UsesAutoExpansion();
		
		// Sincroniza a row com o estado atual da coleção.
		WBP_Entry_RowLayout->BP_OnItemExpansionChanged(
			bExpansionState, 
			bIsExpanded
			);
		
		// Atualiza o visual do botão de expansão da row.
		WBP_Entry_RowLayout->RefreshExpansionButtonVisual();
	}
	
	// Atualiza o toggle action, já que a expansão pode afetar a visibilidade/hint.
	RefreshToggleActionPresentation();
}

void UWidget_ListEntry_Base::SelectThisEntryWidget() const
{
	// Seleciona o item associado na Lista.
	CastChecked<UListView>(GetOwningListView())->SetSelectedItem(GetListItem());
}

int32 UWidget_ListEntry_Base::GetOwningDataHierarchyDepth() const
{
	// Retorna a profundidade hierárquica do DataObject associado.
	return OwningListDataObject ? OwningListDataObject->GetEntryHierarchyDepth() : 0;
}

void UWidget_ListEntry_Base::
SyncTreeExpansion(UListDataObject_Base* InDataObject, const bool bNewExpansion) const
{
	// Aborta se o DataObject for invalido ou se o DataObject não implementa a Interface IListDataWithChildren
	if (!InDataObject || !InDataObject->HasAnyChildListData()) return;
	
	// Só permite alterar expansão via árvore se a entry suportar toggle manual ou auto expansão.
	const bool bCanUseTreeExpansion = 
		OwningListDataObject->CanUserToggleExpansion() ||
		OwningListDataObject->UsesAutoExpansion();
	
	// Aborta se este tipo de expansão não for permitido.
	if (!bCanUseTreeExpansion) return;
	
	// Atualiza o valor Expandido do DataObject
	InDataObject->SetbIsExpanded(bNewExpansion);
	
	// Atualiza a Expansão visual na TreeView
	CastChecked<UCommonTreeView>(GetOwningListView())->SetItemExpansion(InDataObject, bNewExpansion);
}

void UWidget_ListEntry_Base::OnToggleActionClicked() const
{
	// Obtém o subsistema de input para identificar se a entrada é via gamepad.
	const UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem();
	const bool bIsGamepad = CommonInputSubsystem && CommonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad;
	
	// No gamepad, só executa a ação se esta entry realmente estiver selecionada.
	if (bIsGamepad && !IsListItemSelected()) return;

	// Executa o toggle apenas se o DataObject for do tipo Value.
	if (UListDataObject_Value* ValueDataObject = Cast<UListDataObject_Value>(OwningListDataObject))
	{
		ValueDataObject->ExecuteToggleAction();
		
		// Sincroniza o estado visual do botão após a execução da ação.
		if (WBP_ToggleAction)
		{
			WBP_ToggleAction->GetToggleButton()->SetIsActionActive(ValueDataObject->GetbIsToggleActionActive());
		}
	}
}

void UWidget_ListEntry_Base::RefreshToggleActionVisual() const
{
	// Aborta se o widget de toggle ou o DataObject estiverem inválidos.
	if (!WBP_ToggleAction || !OwningListDataObject) return;
	
	// Só atualiza se o DataObject for do tipo Value.
	if (const UListDataObject_Value* ValueDataObject = Cast<UListDataObject_Value>(OwningListDataObject)){
		// Sincroniza apenas o estado ativo/inativo do botão.
		WBP_ToggleAction->GetToggleButton()->SetIsActionActive(ValueDataObject->GetbIsToggleActionActive());
	}
}

void UWidget_ListEntry_Base::RefreshToggleActionPresentation() const
{
	// Aborta se o widget de toggle ou o DataObject estiverem inválidos.
	if (!WBP_ToggleAction || !OwningListDataObject) return;

	// Só processa se o DataObject for do tipo Value.
	if (const UListDataObject_Value* ValueDataObject = Cast<UListDataObject_Value>(OwningListDataObject))
	{
		// Sincroniza visibilidade e estado do botão de toggle.
		WBP_ToggleAction->GetToggleButton()->SetVisibilityByActionType(ValueDataObject);
		WBP_ToggleAction->GetToggleButton()->SetIsActionActive(ValueDataObject->GetbIsToggleActionActive());

		// Verifica se o input atual é gamepad.
		const bool bIsGamepad = GetInputSubsystem() && GetInputSubsystem()->GetCurrentInputType() == ECommonInputType::Gamepad;
		
		// Verifica se esta entry é realmente o item selecionado na ListView.
		const UListView* OwningListView = Cast<UListView>(GetOwningListView());
		const bool bIsActuallySelected = OwningListView && OwningListView->GetSelectedItem() == GetListItem();

		// O hint só deve aparecer com gamepad e entry selecionada.
		const bool bShouldShowToggleActionHint = bIsGamepad && bIsActuallySelected;

		// Mostra ou esconde o hint de input conforme a condição acima.
		if (UCommonActionWidget* ActionWidget = WBP_ToggleAction->GetToggleActionWidget())
		{
			ActionWidget->SetVisibility(
				bShouldShowToggleActionHint
					? ESlateVisibility::SelfHitTestInvisible
					: ESlateVisibility::Collapsed);
		}
	}
	
}

void UWidget_ListEntry_Base::RegisterToggleActionBinding()
{
	// Registra o input action apenas se ainda não estiver registrado e a action estiver configurada.
	if (!ToggleEntryActionHandle.IsValid() && ToggleInputAction)
	{
		ToggleEntryActionHandle = RegisterUIActionBinding(
			FBindUIActionArgs(
				ToggleInputAction,
				false,
				FSimpleDelegate::CreateUObject(this, &UWidget_ListEntry_Base::OnToggleActionClicked)
				));
	}
}

void UWidget_ListEntry_Base::UnregisterToggleActionBinding()
{
	// Remove o binding do input action, se estiver ativo.
	if (ToggleEntryActionHandle.IsValid())
	{
		ToggleEntryActionHandle.Unregister();
		ToggleEntryActionHandle = FUIActionBindingHandle();
	}
}

// DvoraGames All Rights Reserved


#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"

#include "CommonInputSubsystem.h"
#include "CommonTextBlock.h"
#include "Components/ListView.h"
#include "Widgets/Components/Widget_EntryRow.h"
#include "Widgets/Options/DataObjects/IListDataWithChildren.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"

void UWidget_ListEntry_Base::NativeOnItemHovered(bool bIsHovered)
{
	// Repassa o estado de hover da entry para o Blueprint.
	BP_OnItemHovered(bIsHovered, IsListItemSelected());
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

		// Verifica se o item tem a Interface responsavel pelos filhos quando existir
		if (InOwningListDataObject->Implements<UIListDataWithChildren>())
		{
			// Lê o estado expansível e expandido do item.
			bool bIsExpansible = IIListDataWithChildren::Execute_GetIsExpandable(OwningListDataObject);
			bool bIsExpanded = IIListDataWithChildren::Execute_GetIsExpanded(OwningListDataObject);
			
			// Sincroniza a row com o estado atual da coleção.
			WBP_Entry_RowLayout->BP_OnItemExpansionChanged(bIsExpansible, bIsExpanded);
		}
	}
}

void UWidget_ListEntry_Base::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData,
	EOptionsListDataModifyReason ModifyReason)
{
	// Intencionalmente vazio na base - subclasses sobrescrevem para atualizar seus controles visuais
}

void UWidget_ListEntry_Base::NativeOnItemExpansionChanged(bool bIsExpanded)
{
	// Executa a lógica padrão da interface base.
	IUserObjectListEntry::NativeOnItemExpansionChanged(bIsExpanded);
	
	// Verifica se a WBP_EntryRow e OwningListDataObject são validos
	if (WBP_Entry_RowLayout && OwningListDataObject)
	{
		// Verifica se o item tem a Interface responsavel pelos filhos quando existir
		if (OwningListDataObject->Implements<UIListDataWithChildren>())
		{
			// Lê o estado atual de expansão.
			const bool bIsExpandable = IIListDataWithChildren::Execute_GetIsExpandable(OwningListDataObject);
			
			// Atualiza o visual da row com o estado atual.
			WBP_Entry_RowLayout->BP_OnItemExpansionChanged(bIsExpandable, bIsExpanded);
		}
	}
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

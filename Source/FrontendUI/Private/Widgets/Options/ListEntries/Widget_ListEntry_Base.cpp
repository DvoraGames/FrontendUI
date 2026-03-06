// DvoraGames All Rights Reserved


#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"

#include "CommonInputSubsystem.h"
#include "CommonTextBlock.h"
#include "Components/ListView.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"

void UWidget_ListEntry_Base::NativeOnItemHovered(bool bIsHovered)
{
	// Repassa ao BP se esta linha entrou/saiu de hover junto com o estado atual de seleção da ListView
	BP_OnItemHovered(bIsHovered, IsListItemSelected());
}

void UWidget_ListEntry_Base::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Executa a lógica padrão da interface base da Unreal
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	
	// Garante que a linha reaproveitada pela ListView volte a ficar visível ao receber um novo item
	SetVisibility(ESlateVisibility::Visible);
	
	// Converte o item genérico da ListView para o DataObject esperado e inicia o setup visual/lógico da entry
	OnOwningListDataObjectSet(CastChecked<UListDataObject_Base>(ListItemObject));
}

void UWidget_ListEntry_Base::NativeOnEntryReleased()
{    
	// Executa a lógica padrão da interface base da Unreal
	IUserObjectListEntry::NativeOnEntryReleased();
	
	// Remove o estado de hover da linha antes dela ser reciclada para outro item.
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
			if (UWidget* WidgetToFocus = BP_GetWidgetToFocusForGamepad())
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
	// Verifica se o widget de texto foi vinculado no Blueprint
	if (CommonText_SettingDisplayName)
	{    
		// Preenche o texto da entry com o DisplayName da configuração (ex: Difficulty, Language, Volume)
		CommonText_SettingDisplayName->SetText(InOwningListDataObject->GetDataDisplayName());
	}
	
    // Evita bind duplicado quando a mesma entry é reciclada/reutilizada pela ListView para o mesmo DataObject
	if (!InOwningListDataObject->OnListDataModified.IsBoundToObject(this))
	{
		// Faz esta entry escutar mudanças do seu DataObject para que subclasses possam ressincronizar a UI
		InOwningListDataObject->OnListDataModified.AddUObject(this, &ThisClass::OnOwningListDataObjectModified);
	}
}

void UWidget_ListEntry_Base::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData,
	EOptionsListDataModifyReason ModifyReason)
{
	// Intencionalmente vazio na base - subclasses sobrescrevem para atualizar seus controles visuais
}

void UWidget_ListEntry_Base::SelectThisEntryWidget() const
{
	// Solicita à ListView dona que selecione o item associado a esta entry
	CastChecked<UListView>(GetOwningListView())->SetSelectedItem(GetListItem());
}



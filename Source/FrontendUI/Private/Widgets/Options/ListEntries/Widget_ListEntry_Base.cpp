// DvoraGames All Rights Reserved


#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"

#include "CommonTextBlock.h"
#include "Components/ListView.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"

void UWidget_ListEntry_Base::NativeOnItemHovered(bool bIsHovered)
{
	/* Repassa o estado de hover ao Blueprint filho, incluindo se o item está selecionado — necessário para que o BP
	possa compor corretamente os visuais de hover + seleção. */
	BP_OnItemHovered(bIsHovered, IsListItemSelected());
}

void UWidget_ListEntry_Base::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Garante que a interface base da Unreal execute sua lógica padrão de inicialização
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	
	// Garante que o widget esteja visível ao ser associado a um item.
	// Importante na reciclagem: o entry pode ter sido ocultado anteriormente.
	SetVisibility(ESlateVisibility::Visible);
	
	/* Faz o cast seguro do item genérico para o nosso formato de DataObject 
	e aciona o evento customizado que as subclasses podem sobrescrever */
	OnOwningListDataObjectSet(CastChecked<UListDataObject_Base>(ListItemObject));
}

void UWidget_ListEntry_Base::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	// Se o widget de texto foi definido no Blueprint, atualiza o texto com o nome do objeto recém-recebido
	if (CommonText_SettingDisplayName)
	{
		CommonText_SettingDisplayName->SetText(InOwningListDataObject->GetDataDisplayName());
	}
	
	/* Registra este Widget como ouvinte das modificações do seu próprio DataObject,
	garantindo que eventos como mudança de valor ou "Reset" sejam refletidos na UI. */
	// O IsBoundToObject evita duplicar a assinatura caso a linha seja reciclada na ListView.	
	if (!InOwningListDataObject->OnListDataModified.IsBoundToObject(this))
	{
		InOwningListDataObject->OnListDataModified.AddUObject(this, &ThisClass::OnOwningListDataObjectModified);
	}
}

void UWidget_ListEntry_Base::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData,
	EOptionsListDataModifyReason ModifyReason)
{
	// Método intencionalmente vazio na base (embora seja virtual para subclasses sobrescreverem).
	/* Aqui seria onde as subclasses atualizariam seus sliders/rotators quando a UI detectasse que o valor interno foi 
	 * modificado por eventos externos ou reset. */
}

void UWidget_ListEntry_Base::SelectThisEntryWidget() const
{
	// Obtém a ListView dona deste entry e força a seleção deste item.
	// CastChecked garante crash claro se a lista não for UListView.
	CastChecked<UListView>(GetOwningListView())->SetSelectedItem(GetListItem());
}



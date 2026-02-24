// DvoraGames All Rights Reserved


#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"

#include "CommonTextBlock.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"

void UWidget_ListEntry_Base::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Garante que a interface base da Unreal execute sua lógica padrão de inicialização
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	
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



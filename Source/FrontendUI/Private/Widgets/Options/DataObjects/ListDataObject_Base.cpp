// DvoraGames All Rights Reserved


#include "Widgets/Options/DataObjects/ListDataObject_Base.h"

// Delega o setup específico para a subclasse via virtual (ex: Carousel carrega opções do save, Collection cria filhos, etc.)
void UListDataObject_Base::InitDataObject()
{
	OnDataObjectInitialized();
}

// Intencionalmente vazio: subclasses sobrescrevem para implementar seu setup específico
void UListDataObject_Base::OnDataObjectInitialized()
{
	
}

// Dispara o delegate para todos os listeners registrados (ex: Widget_ListEntry_Base)
void UListDataObject_Base::NotifyListDataModified(UListDataObject_Base* ModifiedData,
	EOptionsListDataModifyReason ModifyReason)
{
	OnListDataModified.Broadcast(ModifiedData, ModifyReason);
}

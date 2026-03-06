// DvoraGames All Rights Reserved


#include "Widgets/Options/DataObjects/ListDataObject_Base.h"

#include "FrontendSettings/FrontendGameUserSettings.h"

void UListDataObject_Base::InitDataObject()
{
	// Delega o setup específico para a subclasse via virtual
	OnDataObjectInitialized();
}

// Intencionalmente vazio: subclasses sobrescrevem para implementar seu setup específico
void UListDataObject_Base::OnDataObjectInitialized()
{
	// Intencionalmente vazio - subclasses sobrescrevem para implementar seu setup específico
}

void UListDataObject_Base::NotifyListDataModified(UListDataObject_Base* ModifiedData,
	EOptionsListDataModifyReason ModifyReason)
{
	// Dispara o delegate para notificar os widgets vinculados para reagirem à mudança
	OnListDataModified.Broadcast(ModifiedData, ModifyReason);
	
	// Se esta opção foi marcada como crítica (true).
	if (bShouldApplyChangeImmediately)
	{
		// Força o GameUserSettings a aplicar e salvar imediatamente
		UFrontendGameUserSettings::Get()->ApplySettings(true);
	}
}

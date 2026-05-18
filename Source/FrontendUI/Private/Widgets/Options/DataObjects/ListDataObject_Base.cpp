// DvoraGames All Rights Reserved


#include "Widgets/Options/DataObjects/ListDataObject_Base.h"

#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"

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

int32 UListDataObject_Base::GetHierarchyDepth() const
{
	int32 HierarchyDepth = 0;
	
	// Começa a contagem a partir do pai hierárquico atual.
	const UListDataObject_Base* CurrentParent = GetParentData();
	
	// Sobe na hierarquia até não haver mais pais.
	while (CurrentParent)
	{
		// Soma 1 na Hierarquia
		HierarchyDepth++;
		
		CurrentParent = CurrentParent->GetParentData();
	}
	
	// Retorna a profundidade da Hierarquia
	return HierarchyDepth;
}

int32 UListDataObject_Base::GetChildIndex() const
{
	// Retorna inválido se esta entry não possuir pai.
	if (!ParentData) return INDEX_NONE;
	
	// Tenta converter o pai em uma collection.
	const UListDataObject_Collection* ParentCollection = Cast<UListDataObject_Collection>(ParentData);
	
	// Retorna indice nulo se o pai não for uma collection.
	if (!ParentCollection) return INDEX_NONE;
	
	// Retorna o índice desta entry em relação ao pai.
	return ParentCollection->GetAllChildListData().IndexOfByKey(this);
}

int32 UListDataObject_Base::GetChildrenCount() const
{
	// Retorna zero se esta entry não possuir pai.
	if (!ParentData) return 0;
	
	// Retorna o último índice válido dentro dos filhos do pai.
	return ParentData->GetAllChildListData().Num() - 1;
}

bool UListDataObject_Base::IsLastChild() const
{	
	// Retorna false se esta entry não possuir pai.
	if (!ParentData) return false;
	
	// retorna true se a entry tem o mesmo índice da última Entry entre os filhos do pai.
	return GetChildIndex() == GetChildrenCount();
}

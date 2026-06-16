// DvoraGames All Rights Reserved


#include "Widgets/Options/DataObjects/ListDataObject_Base.h"

#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Widgets/Options/DataObjects/ListDataObject_Bool.h"
#include "Widgets/Options/DataObjects/ListDataObject_SubCategory.h"
#include "Widgets/Options/DataObjects/ListDataObject_TabCollection.h"

void UListDataObject_Base::InitDataObject()
{
	// Delega o setup específico para a subclasse via virtual
	OnDataObjectInitialized();
}

void UListDataObject_Base::OnDataObjectInitialized()
{
	// Intencionalmente vazio - subclasses sobrescrevem para implementar seu setup específico
}

void UListDataObject_Base::NotifyListDataModified(UListDataObject_Base* ModifiedData,
	EOptionsListDataModifyReason ModifyReason)
{
	// Notifica os listeners sobre a alteração.
	OnListDataModified.Broadcast(ModifiedData, ModifyReason);
	
	// Aplica as configurações imediatamente quando necessário.
	if (bShouldApplyChangeImmediately)
	{
		// Força o GameUserSettings a aplicar e salvar imediatamente
		UFrontendGameUserSettings::Get()->ApplySettings(true);
	}
}

int32 UListDataObject_Base::GetEntryHierarchyDepth() const
{
	// Armazena a profundidade da entry atual.
	int32 Depth = 0;
	
	// Começa a contagem a partir do pai atual.
	const UListDataObject_Base* CurrentParent = GetParentData();
	
	// Percorre a hierarquia até chegar no primeiro pai que herde TabCollection.
	while (CurrentParent)
	{
		// Para o loop se o pai atual na hierarquia é um TabCollection
		if (CurrentParent->IsA<UListDataObject_TabCollection>()) break;
		
		// Soma 1 na Hierarquia
		Depth++;
		
		// Avança para o próximo pai.
		CurrentParent = CurrentParent->GetParentData();
	}
	
	// Retorna a profundidade encontrada.
	return Depth;
}

int32 UListDataObject_Base::GetChildIndex() const
{
	// Retorna inválido se o item não possuir pai.
	if (!ParentData) return INDEX_NONE;
	
	// Obtém os irmãos do item atual.
	const TArray<UListDataObject_Base*> Siblings = ParentData->GetAllChildListData();
	
	// Retorna o índice do item entre os irmãos.
	return Siblings.IndexOfByKey(this);
}

bool UListDataObject_Base::IsLastChild() const
{	
	// Retorna false se o item não possuir pai.
	if (!ParentData) return false;
	
	// Obtém os irmãos do item atual.
	TArray<UListDataObject_Base*> Siblings = ParentData->GetAllChildListData();
	
	// Retorna se o item é o último filho.
	return Siblings.Last() == this;
}

bool UListDataObject_Base::IsSubItem() const
{
	const bool bIsSubCategoryItem = ParentData->IsA<UListDataObject_SubCategory>();
	const bool bIsBoolChild = ParentData->IsA<UListDataObject_Bool>();
	
	// Retorna se o pai for uma subcategoria.
	return ParentData && bIsSubCategoryItem || bIsBoolChild;
}

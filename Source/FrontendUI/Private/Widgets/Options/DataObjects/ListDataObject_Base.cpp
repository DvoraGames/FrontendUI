// DvoraGames All Rights Reserved


#include "Widgets/Options/DataObjects/ListDataObject_Base.h"

#include "FrontendSettings/FrontendGameUserSettings.h"
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
	const EOptionsListDataModifyReason ModifyReason)
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

//** ----------------------------------
//** Children Manager
//** ----------------------------------

void UListDataObject_Base::AddChildListData(UListDataObject_Base* InChildListData)
{
	// Aborta se o filho for inválido.
	if (!InChildListData) return;
	
	// Define esta coleção como pai do filho.
	InChildListData->SetParentData(this);
	
    // Armazena a opção filha na coleção interna desta entry.
	ChildListDataArray.Add(InChildListData);
	
	// Inicializa o DataObject filho.
	InChildListData->InitDataObject();
}

TArray<UListDataObject_Base*> UListDataObject_Base::GetAllChildListData() const
{
	// Retorna todos os filhos da coleção.
	return ChildListDataArray;
}

bool UListDataObject_Base::HasAnyChildListData() const
{
	// Retorna se existem filhos registrados.
	return !ChildListDataArray.IsEmpty();
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
	TArray<UListDataObject_Base*> Children = ParentData->GetAllChildListData();
	
	// Retorna false se o pai não possuir filhos válidos.
	if (Children.IsEmpty()) return false;
	
	// Retorna se o item é o último filho.
	return Children.Last() == this;
}

void UListDataObject_Base::SetChildrenMode(const EEntryChildrenMode InChildrenMode)
{
	// Define o modo de comportamento da entry.
	ChildrenMode = InChildrenMode;
	
	// Mantem os filhos da entry sempre visiveis e ativos se o modo de for Always Visible (Sempre visivel).
	if (ChildrenMode == EEntryChildrenMode::AlwaysVisible)
	{
		bIsExpanded = true;
		bChildrenAreActive = true;
	}
}

bool UListDataObject_Base::IsSubItem() const
{
	// Retorna false se o item não possuir pai.
	if (!ParentData) return false;
	
	// Define a variavel como true se a classe pai for Category
	const bool bParentIsCategory = ParentData->IsA<UListDataObject_Category>();
	
	// Define a variavel como true se a classe pai for Subcategory
	const bool bParentIsSubCategory = ParentData->IsA<UListDataObject_SubCategory>();
	
	// Retorna true se o Pai não for uma Category ou se for uma SubCategory (SubCategory herda Category).
	return !bParentIsCategory || bParentIsSubCategory;
}

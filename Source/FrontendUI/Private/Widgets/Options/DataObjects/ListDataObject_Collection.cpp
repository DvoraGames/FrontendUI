// DvoraGames All Rights Reserved

#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"

void UListDataObject_Collection::OnDataObjectInitialized()
{
	// Executa a inicialização da classe base.
	Super::OnDataObjectInitialized();
}

void UListDataObject_Collection::AddChildListData(UListDataObject_Base* InChildListData)
{
	// Aborta se o filho for inválido.
	if (!InChildListData) return;
	
	// Define esta coleção como pai do filho.
	InChildListData->SetParentData(this);
	
	// Armazena a opção filha a coleção interna desta aba.
	ChildListDataArray.Add(InChildListData);
	
	// Inicializa o DataObject filho.
	InChildListData->InitDataObject();
}

TArray<UListDataObject_Base*> UListDataObject_Collection::GetAllChildListData() const
{
	// Retorna todos os filhos da coleção.
	return ChildListDataArray;
}

bool UListDataObject_Collection::HasAnyChildListData() const
{
	// Retorna se existem filhos registrados.
	return !ChildListDataArray.IsEmpty();
}


// DvoraGames All Rights Reserved

#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"

void UListDataObject_Collection::AddChildListData(UListDataObject_Base* InChildListData)
{
	// Inicializa a opção filha - dispara o OnDataObjectInitialized da subclasse
	InChildListData->InitDataObject();
	
	// Registra esta aba como pai da opção filha
	InChildListData->SetParentData(this);
	
	// Adiciona a opção filha ao array interno desta aba
	ChildListDataArray.Add(InChildListData);
}

TArray<UListDataObject_Base*> UListDataObject_Collection::GetAllChildListData() const
{
	// Retorna todas as opções pertencentes a esta aba
	return ChildListDataArray;
}

bool UListDataObject_Collection::HasAnyChildListData() const
{
	// Retorna true se a lista n estiver vazia
	return !ChildListDataArray.IsEmpty();
}

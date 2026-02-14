// DvoraGames All Rights Reserved

#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"

void UListDataObject_Collection::AddChildListData(UListDataObject_Base* InChildListData)
{
	// Inicializa a opção filha
	InChildListData->InitDataObject();
	
	// Fala para opção "seu pai é essa aba"
	InChildListData->SetParentData(this);
	
	// Adiciona na minha lista de opções da aba
	ChildListDataArray.Add(InChildListData);
}

// Retorna TODAS as opções da aba
TArray<UListDataObject_Base*> UListDataObject_Collection::GetAllChildListData() const
{
	return ChildListDataArray;
}

// Retorna true se a aba tem pelo menos 1 opção
bool UListDataObject_Collection::HasAnyChildListData() const
{
	return !ChildListDataArray.IsEmpty();
}

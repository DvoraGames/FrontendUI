// DvoraGames All Rights Reserved


#include "Widgets/Options/DataObjects/ListDataObject_Base.h"

// Chama o virtual pra cada tipo de opção fazer seu setup específico (volume carrega save, collection cria filhos, etc)
void UListDataObject_Base::InitDataObject()
{
	OnDataObjectInitialized();
}

// [EU MESMO] Vazio aqui porque é a base - não faz nada. Filhos sobrescrevem quando necessário
void UListDataObject_Base::OnDataObjectInitialized()
{
	
}

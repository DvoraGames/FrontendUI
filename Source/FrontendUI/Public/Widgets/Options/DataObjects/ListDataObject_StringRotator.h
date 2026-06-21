// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ListDataObject_String.h"
#include "ListDataObject_StringRotator.generated.h"

/**
* UListDataObject_StringRotator
*
* DataObject para opções de menu do tipo Carrossel.
* Permite navegar ciclicamente (anterior/próximo) entre uma lista de opções pré-definidas.
* Separa o valor interno (FString) do texto traduzível exibido ao jogador (FText).
*/
UCLASS()
class FRONTENDUI_API UListDataObject_StringRotator : public UListDataObject_String
{
	GENERATED_BODY()
	
public:	
	// Avança para a próxima opção da lista - volta ao início ao ultrapassar o limite.
	void AdvanceToNextOption();
	
	// Retorna para a opção anterior da lista - vai para o fim ao ultrapassar o limite.
	void BackToPreviousOption();
	
	// Atualiza o valor selecionado a partir de um texto - chamado pelo componente visual Rotator.
	void OnRotatorInitiatedValueChange(const FText& InNewSelectedText);
};

// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "ListDataObject_Collection.generated.h"

/**
* UListDataObject_Collection
*
* DataObject estrutural que representa uma aba de opções.
* Não armazena valor - apenas agrupa opções filhas (ex. Volume, SFX, Dificuldade).
*/
UCLASS(Abstract)
class FRONTENDUI_API UListDataObject_Collection : public UListDataObject_Base
{
	GENERATED_BODY()
	
public:
	// Inicializa e registra uma opção filha dentro desta aba.
	void AddChildListData(UListDataObject_Base* InChildListData);
	
	// Retorna o array com todas as opções filhas desta aba.
	virtual TArray<UListDataObject_Base*> GetAllChildListData() const override;   
	
    // Retorna true se esta aba possui pelo menos uma opção filha.
	virtual bool HasAnyChildListData() const override;
	//~ End UListDataObject_Base Interface

private:
	// Lista de opções pertencentes a esta aba - reconstruída em runtime, não serializada.
	UPROPERTY(Transient)
	TArray<UListDataObject_Base*> ChildListDataArray;
};

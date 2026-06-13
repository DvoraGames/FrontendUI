// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IListDataWithChildren.generated.h"

class UListDataObject_Base;

// Interface UObject para tipos que possuem filhos.
UINTERFACE(MinimalAPI)
class UIListDataWithChildren : public UInterface
{
	GENERATED_BODY()
};

/**
* IIListDataWithChildren
*
* Interface usada por DataObjects que agrupam filhos.
* Expõe operações de hierarquia e expansão.
*/
class FRONTENDUI_API IIListDataWithChildren
{
	GENERATED_BODY()

public:
	// Adiciona um filho à coleção.
	virtual void AddChildListData(UListDataObject_Base* InChildListData) = 0;
	
	// Retorna todos os filhos da coleção.
	virtual TArray<UListDataObject_Base*> GetAllChildListData() const = 0;
	
	// Retorna se existem filhos registrados.
	virtual bool HasAnyChildListData() const = 0;
	
	// Define se o item pode ser expandido.
	virtual void SetIsExpandable(bool bInIsExpandable) = 0;
	
	// Define se o item está expandido.
	virtual void SetIsExpanded(bool bInIsExpanded) = 0;
	
	// Retorna se o item pode ser expandido.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GetIsExpandable() const;
	
	// Retorna se o item está expandido.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GetIsExpanded() const;
};

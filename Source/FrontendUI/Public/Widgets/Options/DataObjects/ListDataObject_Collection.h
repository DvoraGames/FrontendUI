// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "ListDataObject_Collection.generated.h"

/**
 * 
 */
UCLASS()
class FRONTENDUI_API UListDataObject_Collection : public UListDataObject_Base
{
	GENERATED_BODY()
	
public:
	// Adiciona uma opção filha (Volume, SFX, etc) na aba
	void AddChildListData(UListDataObject_Base* InChildListData);
	
	//~ Begin UListDataObject_Base Interface
	// Retorna array com TODAS minhas opções filhas da ABA (override da base que era vazia)
	virtual TArray<UListDataObject_Base*> GetAllChildListData() const override;    
	// Retorna true se tem pelo menos 1 opção filha (override da base que era false)
	virtual bool HasAnyChildListData() const override;
	//~ End UListDataObject_Base Interface
	
private:
	// Lista privada das opções da Aba
	UPROPERTY(Transient)
	TArray<UListDataObject_Base*> ChildListDataArray;
};

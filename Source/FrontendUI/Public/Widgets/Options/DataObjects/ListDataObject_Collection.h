// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "IListDataWithChildren.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "ListDataObject_Collection.generated.h"

/**
* UListDataObject_Collection
*
* DataObject estrutural que representa uma aba de opções.
* Não armazena valor - apenas agrupa opções filhas (ex. Volume, SFX, Dificuldade).
*/
UCLASS(Abstract)
class FRONTENDUI_API UListDataObject_Collection : public UListDataObject_Base, public IIListDataWithChildren
{
	GENERATED_BODY()
	
public:
	//~ Start IIListDataWithChildren Interface
	// Adiciona um filho à coleção.
	virtual void AddChildListData(UListDataObject_Base* InChildListData) override;
	
	// Retorna todos os filhos da coleção.
	virtual TArray<UListDataObject_Base*> GetAllChildListData() const override;
	
	// Retorna se a coleção possui filhos.
	virtual bool HasAnyChildListData() const override;
	
	// Define se a coleção pode ser expandida.
	virtual void SetIsExpandable(bool bInIsExpandable) override { bIsExpandable = bInIsExpandable; }
	
	// Define se a coleção está expandida.
	virtual void SetIsExpanded(bool bInIsExpanded) override { bIsExpanded = bInIsExpanded; }

	// Retorna se a coleção pode ser expandida.
	virtual bool GetIsExpandable_Implementation() const override { return bIsExpandable; }

	// Retorna se a coleção está expandida.
	virtual bool GetIsExpanded_Implementation() const override { return bIsExpanded; }
	//~ End IIListDataWithChildren Interface
	
protected:
	// Executa a inicialização específica da coleção.
	virtual void OnDataObjectInitialized() override;

private:
	// Lista de opções pertencentes a esta aba - reconstruída em runtime, não serializada.
	UPROPERTY(Transient)
	TArray<UListDataObject_Base*> ChildListDataArray;
	
	bool bIsExpandable = false;				// Define se a coleção pode ser expandida.
	bool bIsExpanded = false; 				// Define se a coleção inicia expandida.
};

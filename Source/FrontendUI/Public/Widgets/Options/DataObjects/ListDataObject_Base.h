// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ListDataObject_Base.generated.h"

// MACRO: LIST_DATA_ACCESSOR
// Cria automaticamente Getter/Setter para qualquer propriedade.
// Uso LIST_DATA_ACCESSOR(Type, MeuNome) -> GetMeuNome() const ou SetMeuNome(Type)
#define LIST_DATA_ACCESSOR(DataType, PropertyName) \
	FORCEINLINE DataType Get##PropertyName() const { return PropertyName; } \
	void Set##PropertyName(DataType In##PropertyName) { PropertyName = In##PropertyName; }

/* OBS: A barra invertida \ no final de cada linha da macro serve para continuar a definição em múltiplas linhas */

/**
 * 
 */
UCLASS()
class FRONTENDUI_API UListDataObject_Base : public UObject
{
	GENERATED_BODY()
	
public:
    // GETTERS/SETTERS Automaticos (via macro LIST_DATA_ACCESSOR)
	LIST_DATA_ACCESSOR(FName, DataID)
	LIST_DATA_ACCESSOR(FText, DataDisplayName)
	LIST_DATA_ACCESSOR(FText, DescriptionRichText)
	LIST_DATA_ACCESSOR(FText, DisabledRichText)
	LIST_DATA_ACCESSOR(TSoftClassPtr<UTexture2D>, SoftDescriptionImage)
	LIST_DATA_ACCESSOR(UListDataObject_Base*, ParentData)
	
	// Função que será usada para chamar a função OnDataObjectInitialized() para subclasses fazerem setup específico
	void InitDataObject();
	
	// Retorna array de filhos na hierarquia (override em subclasses se houver filhos). Vazio na classe Base (Esta)
	virtual TArray<UListDataObject_Base*> GetAllChildListData() const { return TArray<UListDataObject_Base*>(); }
	
	// Verifica se possui filhos (false por padrão, override se necessário)
	virtual bool HasAnyChildListData() const { return false; }
	
protected:	
	//Função usada por subclasses para implementar a lógica específica de inicialização
	virtual void OnDataObjectInitialized();
	
private:
	// Propriedades
	FName DataID;									// ID único da configuração 
	FText DataDisplayName;							// Nome exibido
	FText DescriptionRichText;						// Descrição com rich text
	FText DisabledRichText;							// Descrição exibida quando item está desabilitado
	TSoftClassPtr<UTexture2D> SoftDescriptionImage; // Imagem descritiva (carregamento sob demanda)
	
	UPROPERTY(Transient)
	UListDataObject_Base* ParentData;				// Referência ao pai (não serializada, reconstruída em runtime)
};

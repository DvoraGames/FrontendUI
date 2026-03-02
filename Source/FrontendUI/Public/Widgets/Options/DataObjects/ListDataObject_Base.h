// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "UObject/Object.h"
#include "ListDataObject_Base.generated.h"

// MACRO: LIST_DATA_ACCESSOR
// Cria automaticamente Getter/Setter para qualquer propriedade.
// Uso LIST_DATA_ACCESSOR(Type, MeuNome) -> GetMeuNome() const ou SetMeuNome(Type)
#define LIST_DATA_ACCESSOR(DataType, PropertyName) \
	FORCEINLINE DataType Get##PropertyName() const { return PropertyName; } \
	void Set##PropertyName(DataType In##PropertyName) { PropertyName = In##PropertyName; }

/* OBS: A barra invertida \ no final de cada linha da macro serve para continuar a definição em múltiplas linhas */


 /***** Objeto de dados base para cada entrada da lista de opções. ****/
 /* Contém os dados comuns a todas as opções (ID, nome, descrição) e um delegate para notificar widgets quando o 
  * estado da opção for modificado.
  * Subclasses implementam comportamentos específicos (ex: Carousel, Toggle, Slider).
  */
UCLASS(Abstract)
class FRONTENDUI_API UListDataObject_Base : public UObject
{
	GENERATED_BODY()
	
public:
	// Delegate disparado quando este DataObject for modificado. Parâmetros: qual objeto mudou e o motivo.
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnListDataModifiedDelegate, UListDataObject_Base*, EOptionsListDataModifyReason);
	FOnListDataModifiedDelegate OnListDataModified;
	
    // GETTERS/SETTERS gerados via LIST_DATA_ACCESSOR
	LIST_DATA_ACCESSOR(FName, DataID)
	LIST_DATA_ACCESSOR(FText, DataDisplayName)
	LIST_DATA_ACCESSOR(FText, DescriptionRichText)
	LIST_DATA_ACCESSOR(FText, DisabledRichText)
	LIST_DATA_ACCESSOR(TSoftObjectPtr<UTexture2D>, SoftDescriptionImage)
	LIST_DATA_ACCESSOR(UListDataObject_Base*, ParentData)
	
	// Função que será usada para chamar a função OnDataObjectInitialized() para subclasses fazerem setup específico
	void InitDataObject();
	
	// Retorna um Array com todos os DataObjects filhos desta entrada. Vazio por padrão; override em subclasses que agrupam filhos.
	virtual TArray<UListDataObject_Base*> GetAllChildListData() const { return TArray<UListDataObject_Base*>(); }
	
	// Retorna se esta entrada possui filhos na hierarquia. False por padrão; override em subclasses que agrupam filhos.
	virtual bool HasAnyChildListData() const { return false; }
	
	void SetShouldApplySettingsImmediately(bool bShouldApplyRightAway) {bShouldApplyChangeImmediately = bShouldApplyRightAway;}
	
protected:	
	// Override em subclasses para implementar lógica específica de inicialização (ex: carregar save, criar filhos).
	virtual void OnDataObjectInitialized();
	
	// Notifica todos os listeners que este DataObject foi modificado.
	virtual void NotifyListDataModified(UListDataObject_Base* ModifiedData, 
		EOptionsListDataModifyReason ModifyReason = EOptionsListDataModifyReason::DirectlyModified);
	
private:
	// Propriedades
	FName DataID;										// ID único da configuração 
	FText DataDisplayName;								// Nome exibido
	FText DescriptionRichText;							// Descrição com rich text
	FText DisabledRichText;								// Descrição exibida quando item está desabilitado
	TSoftObjectPtr<UTexture2D> SoftDescriptionImage;	// Imagem descritiva (carregamento sob demanda)
	
	// Referência ao pai (não serializada, reconstruída em runtime)
	UPROPERTY(Transient)
	UListDataObject_Base* ParentData;
	
	// Determina se a alteração desta opção deve ser salva e aplicada ao jogo imediatamente. Útil para opções críticas (ex: Resolução de Tela).
	bool bShouldApplyChangeImmediately = false;		
};

// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "UObject/Object.h"
#include "ListDataObject_Base.generated.h"

/** MACRO: LIST_DATA_ACCESSOR
 * 
 * Macro que gera automaticamente Getter e Setter para qualquer propriedade
 * Uso: LIST_DATA_ACCESSOR(FText, DataDisplayName) → GetDataDisplayName() / SetDataDisplayName()
 */
/*? OBS: A barra invertida \ no final de cada linha da macro serve para continuar a definição em múltiplas linhas */
#define LIST_DATA_ACCESSOR(DataType, PropertyName) \
	FORCEINLINE DataType Get##PropertyName() const { return PropertyName; } \
	void Set##PropertyName(DataType In##PropertyName) { PropertyName = In##PropertyName; }



/**
* UListDataObject_Base
*
* Objeto de dados base para cada entrada da lista de opções.
* Contém os dados comuns a todas as opções: ID, nome, descrição e
* um delegate para notificar widgets quando o estado da opção for modificado.
*
* Subclasses implementam comportamentos específicos — ex: Carousel, Toggle, Slider.
*/
UCLASS(Abstract)
class FRONTENDUI_API UListDataObject_Base : public UObject
{
	GENERATED_BODY()
	
public:
	// Delegate disparado quando este DataObject for modificado
	DECLARE_MULTICAST_DELEGATE_TwoParams(
		FOnListDataModifiedDelegate,							// Nome do Delegate
		UListDataObject_Base*, EOptionsListDataModifyReason);	// Qual objeto mudou e o motivo. (Tipo e Enum)
	FOnListDataModifiedDelegate OnListDataModified;
	
	// ----------------------------------------------------------
	// Getters e Setters — gerados via LIST_DATA_ACCESSOR
	// ----------------------------------------------------------
	LIST_DATA_ACCESSOR(FName, DataID)
	LIST_DATA_ACCESSOR(FText, DataDisplayName)
	LIST_DATA_ACCESSOR(FText, DescriptionRichText)
	LIST_DATA_ACCESSOR(FText, DisabledRichText)
	LIST_DATA_ACCESSOR(TSoftObjectPtr<UTexture2D>, SoftDescriptionImage)
	LIST_DATA_ACCESSOR(UListDataObject_Base*, ParentData)
	
	// Chama OnDataObjectInitialized - ponto de entrada para subclasses fazerem seu setup específico.
	void InitDataObject();
	
	// Retorna array com todos os DataObjects filhos - vazio por padrão, override em subclasses que agrupam filhos.
	virtual TArray<UListDataObject_Base*> GetAllChildListData() const { return TArray<UListDataObject_Base*>(); }
	
	// Retorna se esta entrada possui filhos - false por padrão, override em subclasses que agrupam filhos.
	virtual bool HasAnyChildListData() const { return false; }
	
	// Define se a alteração desta opção deve ser aplicada imediatamente.
	void SetShouldApplySettingsImmediately(const bool bShouldApplyRightAway) { bShouldApplyChangeImmediately = bShouldApplyRightAway; }
	
	// As classes filhas devem sobrescrever essas funções para implementar a lógica de Reset.
	virtual bool HasDefaultValue() const { return false; }
	virtual bool CanResetBackToDefaultValue() const { return false; }
	virtual bool TryResetBackToDefaultValue() { return false; }
	
protected:	
	// Override em subclasses para implementar lógica específica de inicialização - ex: carregar save, criar filhos.
	virtual void OnDataObjectInitialized();
	
	// Notifica todos os listeners que este DataObject foi modificado.
	virtual void NotifyListDataModified(UListDataObject_Base* ModifiedData, 
		EOptionsListDataModifyReason ModifyReason = EOptionsListDataModifyReason::DirectlyModified);
	
private:
	
	// ----------------------------------------------------------
	// Properties
	// ----------------------------------------------------------
	
	FName DataID;										// ID único da configuração 
	FText DataDisplayName;								// Nome exibido na lista
	FText DescriptionRichText;							// Descrição com suporte a RichText
	FText DisabledRichText;								// Texto exibido quando a opção está desabilitada
	TSoftObjectPtr<UTexture2D> SoftDescriptionImage;	// Imagem descritiva - carregada sob demanda (lazy)
	
	// Referência ao pai na hierarquia - não serializada, reconstruída em runtime
	UPROPERTY(Transient)
	UListDataObject_Base* ParentData;
	
	// Se true, força o GameUserSettings a aplicar e salvar a mudança imediatamente ao modificar esta opção
	bool bShouldApplyChangeImmediately = false;		
};

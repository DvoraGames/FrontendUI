// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "UObject/Object.h"
#include "ListDataObject_Base.generated.h"

enum class EEntryChildrenMode : uint8;
enum class EToggleActionType : uint8;

/** MACRO: LIST_DATA_ACCESSOR
 * 
 * Macro que gera automaticamente Getter e Setter para qualquer propriedade
 * Uso: LIST_DATA_ACCESSOR(FText, DataDisplayName) → GetDataDisplayName() / SetDataDisplayName()
 */
/*? OBS: A barra invertida \ no final de cada linha da macro serve para continuar a definição em múltiplas linhas */
#define LIST_DATA_ACCESSOR(DataType, PropertyName) \
	FORCEINLINE DataType Get##PropertyName() const { return PropertyName; } \
	void Set##PropertyName(DataType In##PropertyName) { PropertyName = In##PropertyName; }

DECLARE_DELEGATE(FOnToggleActionClicked);

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
	//** ----------------------------------
	//** Base
	//** ----------------------------------

	// Delegate disparado quando este DataObject for modificado
	DECLARE_MULTICAST_DELEGATE_TwoParams(
		FOnListDataModifiedDelegate, // Nome do Delegate
		UListDataObject_Base*, EOptionsListDataModifyReason); // Qual objeto mudou e o motivo. (Tipo e Enum)
	FOnListDataModifiedDelegate OnListDataModified;

	/** Getters e Setters **/
	LIST_DATA_ACCESSOR(FName, DataID)
	LIST_DATA_ACCESSOR(FText, DataDisplayName)
	LIST_DATA_ACCESSOR(FText, DescriptionRichText)
	LIST_DATA_ACCESSOR(FText, DisabledRichText)
	LIST_DATA_ACCESSOR(TSoftObjectPtr<UTexture2D>, SoftDescriptionImage)

	// Inicializa o DataObject.
	void InitDataObject();

	// Define se a alteração deve ser aplicada imediatamente.
	void SetShouldApplySettingsImmediately(const bool bShouldApplyRightAway)
	{
		bShouldApplyChangeImmediately = bShouldApplyRightAway;
	}

	// Retorna se o item possui valor padrão.
	virtual bool HasDefaultValue() const { return false; }

	// Retorna se o item pode ser resetado.
	virtual bool CanResetBackToDefaultValue() const { return false; }

	// Retorna a tentativa de resetar o item para o valor padrão.
	virtual bool TryResetBackToDefaultValue() { return false; }

	//** ----------------------------------
	//** Children Manager
	//** ----------------------------------

	/** Getters e Setters **/
	LIST_DATA_ACCESSOR(UListDataObject_Base*, ParentData)
	LIST_DATA_ACCESSOR(bool, bIsExpanded)
	LIST_DATA_ACCESSOR(bool, bChildrenAreActive)
	
	// Define como os filhos desta entry devem se comportar na TreeView.
	UFUNCTION(BlueprintCallable)
	void SetChildrenMode (const EEntryChildrenMode InChildrenMode);
	
	// Retorna o modo atual de comportamento dos filhos desta entry.
	UFUNCTION(BlueprintPure)
	FORCEINLINE EEntryChildrenMode GetEntryChildrenMode() const { return ChildrenMode; }

	// Retorna se o usuário pode expandir/colapsar manualmente esta entry.
	UFUNCTION(BlueprintPure)
	bool CanUserToggleExpansion() const { return ChildrenMode == EEntryChildrenMode::ExpandableManual; }

	// Retorna se a expansão desta entry é controlada automaticamente.
	UFUNCTION(BlueprintPure)
	bool UsesAutoExpansion() const { return ChildrenMode == EEntryChildrenMode::ExpandableAuto; }

	// Retorna se os filhos devem ficar sempre visíveis (Propria Entry não é selecionavel).
	UFUNCTION(BlueprintPure)
	bool UsesAlwaysVisibleChildren() const { return ChildrenMode == EEntryChildrenMode::AlwaysVisible; }

	// Retorna se o item pertence a uma subcategoria.
	UFUNCTION(BlueprintPure)
	bool IsSubItem() const;
	
	// Retorna o pai para uso em Blueprint.
	UFUNCTION(BlueprintPure, DisplayName="GetParentData")
	UListDataObject_Base* BP_GetParentData() const { return ParentData; }

	// Adiciona um filho à coleção.
	virtual void AddChildListData(UListDataObject_Base* InChildListData);

	// Retorna todos os filhos da coleção.
	virtual TArray<UListDataObject_Base*> GetAllChildListData() const;

	// Retorna se a coleção possui filhos.
	UFUNCTION(BlueprintPure)
	virtual bool HasAnyChildListData() const;

	// Retorna a profundidade do item na hierarquia.
	UFUNCTION(BlueprintPure)
	int32 GetEntryHierarchyDepth() const;

	// Retorna o índice do item dentro do pai.
	UFUNCTION(BlueprintPure)
	int32 GetChildIndex() const;

	// Retorna se o item é o último filho.
	UFUNCTION(BlueprintPure)
	bool IsLastChild() const;

protected:
	//** ----------------------------------
	//** Base
	//** ----------------------------------

	// Override em subclasses para implementar lógica específica de inicialização.
	virtual void OnDataObjectInitialized();

	// Notifica todos os listeners que este DataObject foi modificado.
	virtual void NotifyListDataModified(UListDataObject_Base* ModifiedData,
	                                    EOptionsListDataModifyReason ModifyReason =
		                                    EOptionsListDataModifyReason::DirectlyModified);

private:
	// Delegate reservado para ações de toggle associadas a este DataObject - EX.: Mute, Expand, etc.
	FOnToggleActionClicked OnToggleActionClicked;

	//** ----------------------------------
	//** Base - Properties
	//** ----------------------------------

	FName DataID;										// ID único da configuração 
	FText DataDisplayName;								// Nome exibido na lista
	FText DescriptionRichText;							// Descrição com suporte a RichText
	FText DisabledRichText;								// Texto exibido quando a opção está desabilitada
	TSoftObjectPtr<UTexture2D> SoftDescriptionImage;	// Imagem descritiva - carregada sob demanda (lazy)

	// Define se a alteração deve ser aplicada imediatamente.
	bool bShouldApplyChangeImmediately = false;
	
	//** ----------------------------------
	//** Children Manager - Properties
	//** ----------------------------------

	// Define como os filhos desta entry se comportam na TreeView.
	EEntryChildrenMode ChildrenMode = EEntryChildrenMode::None;
	
	// Armazena se a entry está atualmente expandida.
	bool bIsExpanded = false;
	
	// Define se os filhos desta entry estão ativos para interação.
	bool bChildrenAreActive = true;

	//** ----------------------------------
	//** Children Manager - Runtime Data
	//** ----------------------------------

	// Armazena a referência ao pai hierárquico do item - não serializada, reconstruída em runtime.
	UPROPERTY(Transient)
	UListDataObject_Base* ParentData;

	// Lista de opções pertencentes a esta entry - reconstruída em runtime, não serializada.
	UPROPERTY(Transient)
	TArray<UListDataObject_Base*> ChildListDataArray;
};

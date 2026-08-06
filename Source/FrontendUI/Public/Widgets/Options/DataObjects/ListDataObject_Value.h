// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "FrontendFunctionLibrary.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "ListDataObject_Value.generated.h"

class FOptionsDataInteractionHelper;

using namespace HelperFunctions;

/**
* UListDataObject_Value
*
* Classe base intermediária para opções que armazenam e interagem com um valor de Settings.
* Separa opções de interação direta (Sliders, Carousels, Toggles) de opções puramente
* estruturais (Collections/Abas) que apenas agrupam filhos.
* Centraliza os Helpers de Reflection (Getter e Setter) dinâmicos.
*/
UCLASS(Abstract)
class FRONTENDUI_API UListDataObject_Value : public UListDataObject_Base
{
	GENERATED_BODY()

public:
	LIST_DATA_ACCESSOR(EToggleActionType, ToggleActionType)
	LIST_DATA_ACCESSOR(bool, bIsToggleActionActive)

	// Associa o Helper responsável por ler o valor atual desta opção no backend via Reflection.
	void SetDataDynamicGetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicGetter);

	// Associa o Helper responsável por injetar um novo valor desta opção no backend via Reflection.
	void SetDataDynamicSetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicSetter);

	// Define o valor padrão desta opção a partir de uma string.
	void SetDefaultValueFromString(const FString& InDefaultValue) { DefaultStringValue = InDefaultValue; }

	//~ Begin UListDataObject_Base Interface
	// Retorna true se um valor padrão foi definido para esta opção.
	virtual bool HasDefaultValue() const override { return DefaultStringValue.IsSet(); }
	//~ End UListDataObject_Base Interface

	// Executa a ação de toggle configurada (Mute, Link, etc).
	void ExecuteToggleAction();

protected:
	// Notifica todos os listeners que este DataObject foi modificado.
	virtual void NotifyListDataModified(UListDataObject_Base* ModifiedData,
		EOptionsListDataModifyReason ModifyReason = EOptionsListDataModifyReason::DirectlyModified) override;

	// Retorna o valor padrão como string - válido apenas se HasDefaultValue() for true.
	FString GetDefaultValueAsString() const { return DefaultStringValue.GetValue(); }

	// Helper que escreve informações no backend do jogo via Reflection.
	TSharedPtr<FOptionsDataInteractionHelper> DataDynamicGetter;

	// Helper que escreve informações no backend do jogo via Reflection.
	TSharedPtr<FOptionsDataInteractionHelper> DataDynamicSetter;

	// Sincroniza o estado ativo/inativo do toggle com o valor atual do backend.
	void SyncToggleActionState();

private:
	// Valor padrão desta opção - não definido até que SetDefaultValueFromString seja chamado.
	TOptional<FString> DefaultStringValue;

	// Guarda o último valor não-zero para poder restaurar depois.
	FString PreviousValue;

	// Tipo de ação de toggle associada a esta opção.
	EToggleActionType ToggleActionType = EToggleActionType::None;
	
	// Estado atual do toggle (ativo/inativo).
	bool bIsToggleActionActive = false;

	// Executa a lógica de mute/unmute.
	void OnExecuteMute();
	
	// Executa a lógica de link (ainda não implementada).
	void OnExcuteLink();
};

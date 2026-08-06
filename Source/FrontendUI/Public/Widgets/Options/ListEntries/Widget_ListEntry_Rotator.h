// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"
#include "Widget_ListEntry_Rotator.generated.h"

class UListDataObject_StringRotator;
class UFrontendCommonRotator;
class UFrontendCommonButtonBase;

/**
* UWidget_ListEntry_Rotator
*
* Entry visual para opções do tipo carrossel dentro da ListView.
* Exibe a opção atual no Rotator e permite navegar entre os valores
* usando os botões anterior/próximo ou o próprio Rotator.
*/
UCLASS(Abstract, BlueprintType, meta=(DisableNaiveTick))
class FRONTENDUI_API UWidget_ListEntry_Rotator : public UWidget_ListEntry_Base
{
	GENERATED_BODY()
	
protected:
	//~ Begin UUserWidget Interface
	// Faz os binds iniciais dos botões e do Rotator quando a entry é criada.
	virtual void NativeOnInitialized() override;
	//~ End UUserWidget Interface

	//~ Begin UWidget_ListEntry_Base Interface
	// Faz o Cache do Cast do DataObject recebido para Rotator e sincroniza o Rotator com ele.
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject) override;	
	
	// Atualiza o Rotator quando o DataObject for modificado externamente ou por navegação.
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, 
		EOptionsListDataModifyReason ModifyReason) override;
	//~ End UWidget_ListEntry_Base Interface

private:
	// Solicita ao DataObject navegar para a opção anterior.
	void OnPreviousClicked() const;
	
    // Solicita ao DataObject navegar para a próxima opção.
	void OnNextClicked() const;
	
	// Reage à mudança de valor feita pelo Rotator.
	void OnRotatorValueChanged(int32 Value, bool bUserInitiated) const;
	
	// Define o modo de navegação do Rotator.
	UPROPERTY(EditDefaultsOnly, Category = "Frontend")
	ERotatorLoopMode NavigationMode;
	
	// ----------------------------------------------------------
	// Bound Widgets
	// ----------------------------------------------------------
	
	// Botão usado para voltar uma opção no carrossel.
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase*  CommonButton_PreviousOption;
	
	// Rotator que exibe e navega entre os textos disponíveis do carrossel.
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonRotator*  CommonRotator_AvailableOptions;
	
    // Botão usado para avançar uma opção no carrossel.
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase*  CommonButton_NextOption;
	
	// ----------------------------------------------------------
	// Runtime Data
	// ----------------------------------------------------------
	
	// Cache do DataObject de carrossel associado a esta entry.
	UPROPERTY(Transient)
	UListDataObject_StringRotator* CachedOwningRotatorDataObject;
};

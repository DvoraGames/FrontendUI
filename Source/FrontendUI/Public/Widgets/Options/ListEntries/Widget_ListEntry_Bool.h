// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"
#include "Widget_ListEntry_Bool.generated.h"

class UListDataObject_Bool;
class UFrontendCommonCarousel;
class UFrontendCommonButtonBase;

/**
 * UWidget_ListEntry_Bool
 *
 * Entry visual para opções do tipo Bool dentro da ListView.
 * Permite alternar entre true/false.
 */
UCLASS()
class FRONTENDUI_API UWidget_ListEntry_Bool : public UWidget_ListEntry_Base
{
	GENERATED_BODY()
	
protected:
	//~ Begin UUserWidget Interface
	// Faz os binds iniciais dos botões e do Rotator quando a entry é criada.
	virtual void NativeOnInitialized() override;
	//~ End UUserWidget Interface

	//~ Begin UWidget_ListEntry_Base Interface
	// Faz o Cache do Cast do DataObject recebido para Carousel e sincroniza o Rotator com ele.
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject) override;	
	
	// Atualiza o Rotator quando o DataObject for modificado externamente ou por navegação.
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, 
		EOptionsListDataModifyReason ModifyReason) override;
	//~ End UWidget_ListEntry_Base Interface
	
	// Atualiza o estado de expansão da categoria.
	virtual void NativeOnItemExpansionChanged(bool bIsExpanded) override;
	
private:
	// ----------------------------------------------------------
	// Handlers
	// ----------------------------------------------------------
	
	// Navega para a opção anterior no Carousel.
	void OnPreviousClicked() const;
	
	// Navega para a próxima opção no Carousel.
	void OnNextClicked() const;
	
	// Notifica o DataObject com o novo valor selecionado após rotação do Carousel.
	void OnCarouselRotated(int32 NewIndex, bool bFromNavigation) const;
	
	// ----------------------------------------------------------
	// Bound Widgets
	// ----------------------------------------------------------
	
	// Botão usado para voltar uma opção no carrossel.
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase* CommonButton_PreviousOption;
	
	// Carousel animado que exibe as opções disponíveis e suporta navegação com loop. (Classe customizada)
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))	
	UFrontendCommonCarousel* CommonCarousel_BoolOptions;
	
	// Botão usado para avançar uma opção no carrossel.
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase* CommonButton_NextOption;
	
	// ----------------------------------------------------------
	// Runtime Data
	// ----------------------------------------------------------
	
	// Cache do DataObject de Bool associado a esta entry.
	UPROPERTY(Transient)
	UListDataObject_Bool* CachedOwningBoolDataObject;
};
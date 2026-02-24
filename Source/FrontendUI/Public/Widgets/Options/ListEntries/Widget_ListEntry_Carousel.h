// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"
#include "Widget_ListEntry_Carousel.generated.h"

class UListDataObject_Carousel;
class UFrontendCommonRotator;
class UFrontendCommonButtonBase;

/*
 * Entrada de lista do tipo carrossel: mostra uma opção atual e permite navegar entre opções.
 * Os botões Anterior/Próximo alteram o estado do UListDataObject_Carousel, e o rotator reflete o texto atual.
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNaiveTick))
class FRONTENDUI_API UWidget_ListEntry_Carousel : public UWidget_ListEntry_Base
{
	GENERATED_BODY()
	
protected:
	//~ Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	//~ End UUserWidget Interface

	//~ Begin UWidget_ListEntry_Base Interface
	/* Cacheia o UListDataObject_Carousel e inicializa o rotator popula a lista de textos (opções)
	 * e aplica o texto atualmente selecionado. */
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject) override;	
	
	/* Quando o DataObject mudar (por navegação ou atualização externa), ressincroniza o rotator
	para exibir o texto atual do carrossel. */
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, 
		EOptionsListDataModifyReason ModifyReason) override;
	//~ End UWidget_ListEntry_Base Interface

private:
    // Solicita ao DataObject voltar para a opção anterior.
	void OnPreviousClicked() const;
    // Solicita ao DataObject avançar para a próxima opção.
	void OnNextClicked() const;
	
	/***** Bound Widgets *****/
    // Botão "Anterior" do carrossel.
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase*  CommonButton_PreviousOption;
	
    // Rotator que lista e exibe as opções disponíveis do carrossel.
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonRotator*  CommonRotator_AvailableOptions;
	
    // Botão "Próximo" do carrossel.
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase*  CommonButton_NextOption;
	/***** Bound Widgets *****/
	
	// DataObject do carrossel associado a esta entrada (cache para evitar casts repetidos).
	UPROPERTY(Transient)
	UListDataObject_Carousel* CachedOwningCarouselDataObject;
};

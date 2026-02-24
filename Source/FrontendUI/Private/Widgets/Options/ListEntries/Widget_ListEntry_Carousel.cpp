// DvoraGames All Rights Reserved


#include "Widgets/Options/ListEntries/Widget_ListEntry_Carousel.h"

#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "Widgets/Components/FrontendCommonRotator.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "Widgets/Options/DataObjects/ListDataObject_Carousel.h"

void UWidget_ListEntry_Carousel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
    // Bind do clique para navegar para a opção anterior no carrossel
	CommonButton_PreviousOption->OnClicked().AddUObject(this, &ThisClass::OnPreviousClicked);
	
    // Bind do clique para navegar para a próxima opção no carrossel
	CommonButton_NextOption->OnClicked().AddUObject(this, &ThisClass::OnNextClicked);
}

void UWidget_ListEntry_Carousel::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);
	
	// Este entry espera um DataObject de carrossel; guarda em cache para uso nos cliques/atualizações
	// Converte e guarda o DataObject como carrossel específico
	CachedOwningCarouselDataObject = CastChecked<UListDataObject_Carousel>(InOwningListDataObject);
	
	// Preenche o rotator com todas as opções disponíveis do carrossel
	CommonRotator_AvailableOptions->PopulateTextLabels(CachedOwningCarouselDataObject->GetAvailableOptionsTextArray());
	
	// Ajusta a seleção atual do rotator para refletir o estado do DataObject
	CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningCarouselDataObject->GetCurrentDisplayText());
}

void UWidget_ListEntry_Carousel::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData,
	EOptionsListDataModifyReason ModifyReason)
{
	Super::OnOwningListDataObjectModified(OwningModifiedData, ModifyReason);
	
	if (CachedOwningCarouselDataObject)
	{
		// Se o DataObject foi atualizado, atualiza o rotator para refletir o texto atual
		CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningCarouselDataObject->GetCurrentDisplayText());
	}
}

void UWidget_ListEntry_Carousel::OnPreviousClicked() const
{
	if (CachedOwningCarouselDataObject)
	{
		// Se o DataObject foi atualizado, atualiza o rotator para refletir o texto atual
		CachedOwningCarouselDataObject->BackToPreviousOption();
	}
}
void UWidget_ListEntry_Carousel::OnNextClicked() const
{
	if (CachedOwningCarouselDataObject)
	{
		// Navega para a próxima opção no DataObject (a UI será atualizada via modificação do DataObject)
		CachedOwningCarouselDataObject->AdvanceToNextOption();
	}
}

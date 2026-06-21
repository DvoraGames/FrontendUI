// DvoraGames All Rights Reserved


#include "Widgets/Options/ListEntries/Widget_ListEntry_Carousel.h"

#include "Blueprint/WidgetTree.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "Widgets/Options/DataObjects/ListDataObject_StringCarousel.h"
#include "Widgets/Components/FrontendCommonCarousel.h"

void UWidget_ListEntry_Carousel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	// Vincula o clique do botão esquerdo para pedir ao DataObject voltar uma opção
	CommonButton_PreviousOption->OnClicked().AddUObject(this, &ThisClass::OnPreviousClicked);
	
	// Vincula o clique do botão direito para pedir ao DataObject avançar uma opção
	CommonButton_NextOption->OnClicked().AddUObject(this, &ThisClass::OnNextClicked);
}

void UWidget_ListEntry_Carousel::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);
	
	// Faz o cast para o tipo específico de carrossel e guarda em cache para uso nos cliques e atualizações
	CachedOwningCarouselDataObject = CastChecked<UListDataObject_StringCarousel>(InOwningListDataObject);
	
	// Popula o Carousel com as opções disponíveis (Classe customizada)
	CommonCarousel_AvailableOptions->PopulateCarouselLabels(CachedOwningCarouselDataObject->GetAvailableOptionsTextArray());
	
	// Seleciona o valor atual do DataObject
	CommonCarousel_AvailableOptions->SetSelectedByText(CachedOwningCarouselDataObject->GetCurrentDisplayText());
	
	// Vincula o delegate de rotação para notificar/atualizar o DataObject ao navegar
	CommonCarousel_AvailableOptions->OnRotatedEvent.AddUObject(this, &ThisClass::OnCarouselRotated);
}

void UWidget_ListEntry_Carousel::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData,
	EOptionsListDataModifyReason ModifyReason)
{
	Super::OnOwningListDataObjectModified(OwningModifiedData, ModifyReason);
	
	// Aborta se o DataObject ainda não foi cacheado
	if (!CachedOwningCarouselDataObject) return;
	
	// Lê o texto atualmente exibido no Carousel
	const FText CurrentText = CommonCarousel_AvailableOptions->GetSelectedText();
	
	// Lê o texto que o DataObject considera como valor atual
	const FText DataText = CachedOwningCarouselDataObject->GetCurrentDisplayText();
	
	// Evita chamar SetSelectedByText desnecessariamente se já estão sincronizados
	if (!CurrentText.EqualTo(DataText))
	{
		// Força o Carousel a exibir o valor do DataObject
		CommonCarousel_AvailableOptions->SetSelectedByText(DataText);
	}
}

void UWidget_ListEntry_Carousel::OnPreviousClicked() const
{
	// Navega para a opção anterior no Carousel
	CommonCarousel_AvailableOptions->ShiftLeft();

	// Garante que esta linha continue/torne-se a selecionada na ListView após o clique
	SelectThisEntryWidget();
}
void UWidget_ListEntry_Carousel::OnNextClicked() const
{
	// Navega para a proxima opção no Carousel
	CommonCarousel_AvailableOptions->ShiftRight();

	// Garante que esta linha continue/torne-se a selecionada na ListView após o clique
	SelectThisEntryWidget();
}

void UWidget_ListEntry_Carousel::OnCarouselRotated(int32 NewIndex, bool bFromNavigation) const
{
	// Aborta se o DataObject ainda não foi cacheado
	if (!CachedOwningCarouselDataObject) return;
	
	// Repassa o texto selecionado no Carousel ao DataObject para persistir a mudança
	CachedOwningCarouselDataObject->OnCarouselInitiatedValueChange(CommonCarousel_AvailableOptions->GetSelectedText());
}

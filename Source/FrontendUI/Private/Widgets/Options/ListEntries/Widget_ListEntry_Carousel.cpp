// DvoraGames All Rights Reserved


#include "Widgets/Options/ListEntries/Widget_ListEntry_Carousel.h"

#include "CommonInputSubsystem.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "Widgets/Components/FrontendCommonRotator.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "Widgets/Options/DataObjects/ListDataObject_Carousel.h"

void UWidget_ListEntry_Carousel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	// Vincula o clique do botão esquerdo para pedir ao DataObject voltar uma opção
	CommonButton_PreviousOption->OnClicked().AddUObject(this, &ThisClass::OnPreviousClicked);
	
	// Vincula o clique do botão direito para pedir ao DataObject avançar uma opção
	CommonButton_NextOption->OnClicked().AddUObject(this, &ThisClass::OnNextClicked);
	
	// Ao clicar no Rotator, esta própria linha vira o item selecionado da ListView
	CommonRotator_AvailableOptions->OnClicked().AddLambda(
		[this]()
		{
			// Seleciona a propria entry
			SelectThisEntryWidget();
		});
	
	// Vincula o evento disparado quando o Rotator troca de opção
	CommonRotator_AvailableOptions->OnRotatedEvent.AddUObject(this, &ThisClass::OnRotatorValueChanged);
}

void UWidget_ListEntry_Carousel::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);
	
	// Faz o cast para o tipo específico de carrossel e guarda em cache para uso nos cliques e atualizações
	CachedOwningCarouselDataObject = CastChecked<UListDataObject_Carousel>(InOwningListDataObject);
	
	// Popula o Rotator com todos os textos disponíveis configurados no DataObject
	CommonRotator_AvailableOptions->PopulateTextLabels(CachedOwningCarouselDataObject->GetAvailableOptionsTextArray());
	
	// Ajusta o Rotator para mostrar o texto atualmente selecionado no DataObject
	CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningCarouselDataObject->GetCurrentDisplayText());
}

void UWidget_ListEntry_Carousel::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData,
	EOptionsListDataModifyReason ModifyReason)
{
	Super::OnOwningListDataObjectModified(OwningModifiedData, ModifyReason);
	
	// Se o DataObject desta entry estiver válido
	if (CachedOwningCarouselDataObject)
	{
		// Atualiza o rotator com o texto atual
		CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningCarouselDataObject->GetCurrentDisplayText());
	}
}

void UWidget_ListEntry_Carousel::OnPreviousClicked() const
{
	// Se o DataObject desta entry estiver válido
	if (CachedOwningCarouselDataObject)
	{
		// Pede ao DataObject voltar para a opção anterior do carrossel
		CachedOwningCarouselDataObject->BackToPreviousOption();
	}
	
	// Garante que esta linha continue/torne-se a selecionada na ListView após o clique
	SelectThisEntryWidget();
}
void UWidget_ListEntry_Carousel::OnNextClicked() const
{
	// Se o DataObject desta entry estiver válido
	if (CachedOwningCarouselDataObject)
	{
		// Navega para a próxima opção no DataObject (a UI será atualizada via modificação do DataObject)
		CachedOwningCarouselDataObject->AdvanceToNextOption();
	}
	
	// Garante que esta linha continue/torne-se a selecionada na ListView após o clique
	SelectThisEntryWidget();
}

void UWidget_ListEntry_Carousel::OnRotatorValueChanged(int32 Value, bool bUserInitiated) const
{
	// Aborta se não houver DataObject em cache
	if (!CachedOwningCarouselDataObject) return;
	
	// Obtém o subsistema de input para descobrir de onde veio a mudança do Rotator
	UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem();
	
	// Ignora mudanças que não vieram do usuário ou quando o subsistema não estiver disponível
	if (!CommonInputSubsystem || !bUserInitiated) return;
	
	// Só propaga a mudança para o DataObject quando a navegação veio do Gamepad
	if (CommonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
	{
		// Envia ao DataObject o texto atualmente selecionado no Rotator
		CachedOwningCarouselDataObject->OnRotatorInitiatedValueChange(CommonRotator_AvailableOptions->GetSelectedText());
	}
}

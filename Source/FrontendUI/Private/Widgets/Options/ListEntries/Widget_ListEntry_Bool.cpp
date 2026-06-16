// DvoraGames All Rights Reserved


#include "Widgets/Options/ListEntries/Widget_ListEntry_Bool.h"

#include "CommonTreeView.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "Widgets/Components/FrontendCommonCarousel.h"
#include "Widgets/Options/DataObjects/ListDataObject_Bool.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"

void UWidget_ListEntry_Bool::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	// Vincula o clique do botão esquerdo para navegar para a opção anterior
	CommonButton_PreviousOption->OnClicked().AddUObject(this, &ThisClass::OnPreviousClicked);
	
	// Vincula o clique do botão direito para navegar para a próxima opção
	CommonButton_NextOption->OnClicked().AddUObject(this, &ThisClass::OnNextClicked);
}

void UWidget_ListEntry_Bool::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);
	
	// Faz o cast para o tipo específico e guarda em cache para uso nos handlers
	CachedOwningBoolDataObject = CastChecked<UListDataObject_Bool>(InOwningListDataObject);
	
	// Popula o Carousel com todos os textos disponíveis configurados no DataObject
	CommonCarousel_BoolOptions->PopulateCarouselLabels(CachedOwningBoolDataObject->GetAvailableOptionsTextArray());
	
	// Ajusta o Carousel para mostrar o texto atualmente selecionado no DataObject
	CommonCarousel_BoolOptions->SetSelectedByText(CachedOwningBoolDataObject->GetCurrentDisplayText());
	
	// Vincula o evento disparado quando o Carousel troca de opção
	CommonCarousel_BoolOptions->OnRotatedEvent.AddUObject(this, &ThisClass::OnCarouselRotated);
}

void UWidget_ListEntry_Bool::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData,
	EOptionsListDataModifyReason ModifyReason)
{
	Super::OnOwningListDataObjectModified(OwningModifiedData, ModifyReason);
	
	// Aborta se o DataObject em cache for inválido
	if (!CachedOwningBoolDataObject) return;
	
	// Obtém o texto atualmente exibido no Carousel
	const FText CurrentText = CommonCarousel_BoolOptions->GetSelectedText();
	
	// Obtém o texto atual do DataObject
	const FText DataText = CachedOwningBoolDataObject->GetCurrentDisplayText();
	
	// Ressincroniza o Carousel apenas se o texto exibido estiver desatualizado
	if (!CurrentText.EqualTo(DataText))
	{
		CommonCarousel_BoolOptions->SetSelectedByText(DataText);
	}
	
	// Aborta a expansão se esta entry não possuir filhos
	if (CachedOwningBoolDataObject->HasAnyChildListData())
	{
		// Lê o estado de expansão armazenado no DataObject via interface
		const bool bShouldBeExpanded = IIListDataWithChildren::Execute_GetIsExpanded(CachedOwningBoolDataObject);
		
		// Aplica o estado de expansão no TreeView
		CastChecked<UCommonTreeView>(GetOwningListView())->SetItemExpansion(CachedOwningBoolDataObject, bShouldBeExpanded);
	}
}

void UWidget_ListEntry_Bool::NativeOnItemExpansionChanged(const bool bIsExpanded)
{
	// Bloqueia a expansão quando a categoria não for expansível.
	if (!IIListDataWithChildren::Execute_GetIsExpandable(CachedOwningBoolDataObject))
	{
		// Força o TreeView a manter o estado original
		CastChecked<UCommonTreeView>(GetOwningListView())->SetItemExpansion(
			CachedOwningBoolDataObject,
			IIListDataWithChildren::Execute_GetIsExpanded(CachedOwningBoolDataObject)
		);
		return;
	}
	
	// Força a expansão quando o bool for true - os filhos devem estar sempre visíveis
	if (CachedOwningBoolDataObject->GetCurrentBoolState() && !bIsExpanded)
	{
		CastChecked<UCommonTreeView>(GetOwningListView())->SetItemExpansion(
			CachedOwningBoolDataObject,
			true
		);
		return;
	}
	
	// Executa o comportamento padrão quando a categoria puder expandir.
	Super::NativeOnItemExpansionChanged(bIsExpanded);
}

void UWidget_ListEntry_Bool::OnPreviousClicked() const
{
	// Navega para a opção anterior no Carousel
	CommonCarousel_BoolOptions->ShiftLeft();

	// Garante que esta linha continue/torne-se a selecionada na ListView após o clique
	SelectThisEntryWidget();
}

void UWidget_ListEntry_Bool::OnNextClicked() const
{
	// Navega para a proxima opção no Carousel
	CommonCarousel_BoolOptions->ShiftRight();

	// Garante que esta linha continue/torne-se a selecionada na ListView após o clique
	SelectThisEntryWidget();
}

void UWidget_ListEntry_Bool::OnCarouselRotated(int32 NewIndex, bool bFromNavigation) const
{
	// Aborta se o DataObject em cache for inválido
	if (!CachedOwningBoolDataObject) return;
	
	// Notifica o DataObject com o texto atualmente selecionado no Carousel
	CachedOwningBoolDataObject->OnCarouselInitiatedValueChange(CommonCarousel_BoolOptions->GetSelectedText());
	
	// Aborta a sincronização de expansão se esta entry não possuir filhos
	if (CachedOwningBoolDataObject->HasAnyChildListData()) return;
	
	// Lê o novo estado bool para determinar se deve expandir ou colapsar
	const bool bNewExpansion = CachedOwningBoolDataObject->GetCurrentBoolState();
		
	// Atualiza o estado de expansão armazenado no DataObject
	CachedOwningBoolDataObject->SetIsExpanded(bNewExpansion);
		
	// Aplica o novo estado de expansão no TreeView
	CastChecked<UCommonTreeView>(GetOwningListView())->SetItemExpansion(CachedOwningBoolDataObject, bNewExpansion);
}

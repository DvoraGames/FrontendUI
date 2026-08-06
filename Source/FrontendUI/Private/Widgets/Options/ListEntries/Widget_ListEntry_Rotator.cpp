// DvoraGames All Rights Reserved


#include "Widgets/Options/ListEntries/Widget_ListEntry_Rotator.h"

#include "CommonInputSubsystem.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "Widgets/Components/FrontendCommonRotator.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "Widgets/Options/DataObjects/ListDataObject_StringRotator.h"

void UWidget_ListEntry_Rotator::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	// Vincula o clique do botão esquerdo para voltar uma opção.
	CommonButton_PreviousOption->OnClicked().AddUObject(this, &ThisClass::OnPreviousClicked);
	
	// Vincula o clique do botão direito para avançar uma opção.
	CommonButton_NextOption->OnClicked().AddUObject(this, &ThisClass::OnNextClicked);
	
	// Faz a entry ser selecionada quando o Rotator for clicado.
	CommonRotator_AvailableOptions->OnClicked().AddLambda(
		[this]()
		{
			// Seleciona a propria entry na ListView
			SelectThisEntryWidget();
		});
	
	// Vincula o evento disparado quando o Rotator muda de valor.
	CommonRotator_AvailableOptions->OnRotatedEvent.AddUObject(this, &ThisClass::OnRotatorValueChanged);
}

void UWidget_ListEntry_Rotator::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);
	
	// Faz o cast para o DataObject do tipo Rotator e armazena em cache.
	CachedOwningRotatorDataObject = CastChecked<UListDataObject_StringRotator>(InOwningListDataObject);
	
	// Popula o Rotator com todos os textos disponíveis configurados no DataObject.
	CommonRotator_AvailableOptions->PopulateTextLabels(CachedOwningRotatorDataObject->GetAvailableOptionsTextArray());
	
	// Sincroniza o texto atual exibido no Rotator.
	CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningRotatorDataObject->GetCurrentDisplayText());
	
	// Aplica o modo de navegação configurado nesta entry.
	CachedOwningRotatorDataObject->SetNavigationMode(NavigationMode);
}

void UWidget_ListEntry_Rotator::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData,
	const EOptionsListDataModifyReason ModifyReason)
{
	Super::OnOwningListDataObjectModified(OwningModifiedData, ModifyReason);
	
	// Atualiza o Rotator se o DataObject em cache estiver válido.
	if (CachedOwningRotatorDataObject)
	{
		// Sincroniza o texto atual no widget.
		CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningRotatorDataObject->GetCurrentDisplayText());
	}
}

void UWidget_ListEntry_Rotator::OnPreviousClicked() const
{
	// Volta para a opção anterior se o DataObject estiver válido.
	if (CachedOwningRotatorDataObject)
	{
		// Navegar para a opção anterior no DataObject do Rotator.
		CachedOwningRotatorDataObject->BackToPreviousOption();
	}
	
	// Mantém esta entry selecionada na ListView após o clique.
	SelectThisEntryWidget();
}

void UWidget_ListEntry_Rotator::OnNextClicked() const
{
	// Avança para a próxima opção se o DataObject estiver válido.
	if (CachedOwningRotatorDataObject)
	{
		// Navegar para a próxima opção no DataObject do Rotator.
		CachedOwningRotatorDataObject->AdvanceToNextOption();
	}
	
	// Mantém esta entry selecionada na ListView após o clique.
	SelectThisEntryWidget();
}

void UWidget_ListEntry_Rotator::OnRotatorValueChanged(int32 Value, bool bUserInitiated) const
{
	// Aborta se não houver DataObject em cache.
	if (!CachedOwningRotatorDataObject) return;
	
	// Obtém o subsistema de input atual.
	UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem();
	
	// Ignora mudanças não iniciadas pelo usuário ou sem subsistema válido.
	if (!CommonInputSubsystem || !bUserInitiated) return;
	
	// Só propaga a mudança quando a entrada vier do gamepad.
	if (CommonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
	{
		// Envia ao DataObject o texto atualmente selecionado no Rotator.
		CachedOwningRotatorDataObject->OnRotatorInitiatedValueChange(CommonRotator_AvailableOptions->GetSelectedText());
	}
}

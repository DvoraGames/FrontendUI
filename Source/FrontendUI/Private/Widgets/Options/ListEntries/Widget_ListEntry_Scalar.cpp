// DvoraGames All Rights Reserved


#include "Widgets/Options/ListEntries/Widget_ListEntry_Scalar.h"

#include "AnalogSlider.h"
#include "Widgets/Options/DataObjects/ListDataObject_Scalar.h"

void UWidget_ListEntry_Scalar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	// Registra o handler de alteração de valor do slider.
	AnalogSlider_SettingSlider->OnValueChanged.AddUniqueDynamic(this, &ThisClass::OnSliderValueChanged);
	
	// Registra o handler de interação do slider.
	AnalogSlider_SettingSlider->OnMouseCaptureBegin.AddUniqueDynamic(this, &ThisClass::OnSliderMouseCaptureBegins);
}

void UWidget_ListEntry_Scalar::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);
	
	// Faz cache do DataObject escalar desta entry.
	CachedOwningScalarDataObject = CastChecked<UListDataObject_Scalar>(InOwningListDataObject);
	
	/*** Numeric Text ***/
	// Sincroniza o tipo numérico com base no valor definido no DataObject.
	CommonNumeric_SettingValue->SetNumericType(CachedOwningScalarDataObject->GetDisplayNumericType());
	// Sincroniza a formatação do texto numérico com base DataObject.
	CommonNumeric_SettingValue->FormattingSpecification = CachedOwningScalarDataObject->GetNumberFormattingOptions();
	// Sincroniza o valor exibido no texto numerico com o do DataObject.
	CommonNumeric_SettingValue->SetCurrentValue(CachedOwningScalarDataObject->GetCurrentValue());
	
	/*** Analog Slider ***/
	// Sincroniza o valor mínimo do slider com base no DataObject.
	AnalogSlider_SettingSlider->SetMinValue(CachedOwningScalarDataObject->GetDisplayValueRange().GetLowerBoundValue());
	// Sincroniza o valor máximo do slider com base nno DataObject.
	AnalogSlider_SettingSlider->SetMaxValue(CachedOwningScalarDataObject->GetDisplayValueRange().GetUpperBoundValue());
	// Sincroniza o valor atual do slider com base no DataObject.
	AnalogSlider_SettingSlider->SetValue(CachedOwningScalarDataObject->GetCurrentValue());	
}

void UWidget_ListEntry_Scalar::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData,
	EOptionsListDataModifyReason ModifyReason)
{
	// Aborta se o DataObject for inválido.
	if (!CachedOwningScalarDataObject) return;
	
	// Atualiza o valor exibido no texto numérico.
	CommonNumeric_SettingValue->SetCurrentValue(CachedOwningScalarDataObject->GetCurrentValue());
	
	// Atualiza o valor atual exibido no slider.
	AnalogSlider_SettingSlider->SetValue(CachedOwningScalarDataObject->GetCurrentValue());
}

void UWidget_ListEntry_Scalar::OnSliderValueChanged(const float Value)
{
	// Aborta se o DataObject for inválido.
	if (!CachedOwningScalarDataObject) return;
	
	// Repassa o novo valor do slider para o DataObject.
	CachedOwningScalarDataObject->SetCurrentValueFromSlider(Value);
}

void UWidget_ListEntry_Scalar::OnSliderMouseCaptureBegins()
{
	// Força a seleção desta entry ao iniciar a interação com o slider.
	SelectThisEntryWidget();
}

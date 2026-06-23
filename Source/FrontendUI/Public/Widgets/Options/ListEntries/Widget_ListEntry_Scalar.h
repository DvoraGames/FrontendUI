// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"
#include "Widget_ListEntry_Scalar.generated.h"

class UListDataObject_Scalar;
class UCommonNumericTextBlock;
class UAnalogSlider;

/**
* UWidget_ListEntry_Scalar
*
* Entry usada para opções numéricas baseadas em slider.
* Sincroniza o valor exibido no texto e no slider com o DataObject_Scalar,
* além de encaminhar as alterações do usuário para o backend.
*/
UCLASS(Abstract, BlueprintType, meta=(DisableNaiveTick))
class FRONTENDUI_API UWidget_ListEntry_Scalar : public UWidget_ListEntry_Base
{
	GENERATED_BODY()
	
protected:
	//~ Begin UUserWidget Interface
	// Registra os binds iniciais do slider.
	virtual void NativeOnInitialized() override;
	//~ End UUserWidget Interface

	//~ Begin UWidget_ListEntry_Base Interface
	// Faz cache do DataObject escalar e sincroniza os widgets.
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject) override;	
	
	// Atualiza o texto e o slider quando o DataObject for modificado.
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, 
		EOptionsListDataModifyReason ModifyReason) override;
	//~ End UWidget_ListEntry_Base Interface
	
private:
	// Handler disparado quando o valor do slider mudar.
	UFUNCTION()
	void OnSliderValueChanged(float Value);
	
	// Handler disparado ao iniciar a interação com o slider.
	UFUNCTION()
	void OnSliderMouseCaptureBegins();
	
	// ----------------------------------------------------------
	// Bound Widgets
	// ----------------------------------------------------------
	
	// Texto numérico que exibe o valor atual da opção.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UCommonNumericTextBlock* CommonNumeric_SettingValue;

	// Slider usado para alterar o valor da opção.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UAnalogSlider*  AnalogSlider_SettingSlider;
	
	// ----------------------------------------------------------
	// Runtime Data
	// ----------------------------------------------------------
	
	// Cache do DataObject escalar associado a esta entry.	
	UPROPERTY(Transient)
	UListDataObject_Scalar* CachedOwningScalarDataObject;
};

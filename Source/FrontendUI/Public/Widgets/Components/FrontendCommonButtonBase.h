// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CommonTextBlock.h"
#include "FrontendCommonButtonBase.generated.h"

/**
* UFrontendCommonButtonBase
*
* Botão base do Frontend - herda de UCommonButtonBase e adiciona
* suporte a texto configurável, uppercase automático, estilo por estado
* e notificação global de descrição ao hover via FrontendUISubsystem.
*/
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class FRONTENDUI_API UFrontendCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()
	
public:
	// Define o texto exibido no botão.
	UFUNCTION(BlueprintCallable)
	void SetButtonText(FText InText);
	
	// Retorna o texto atualmente exibido no botão.
	UFUNCTION(BlueprintCallable)
	FText GetButtonDisplayText() const;
	
private:
	//~ Begin UUserWidget Interface
	// Aplica o texto padrão (ButtonText) antes de renderizar no editor/runtime.
	virtual void NativePreConstruct() override;
	//~ End UUserWidget Interface
	
	//~ Begin UCommonButton Interface
	// Chamado quando o estilo de texto do botão muda (Normal, Hover, Selected, Disabled)k.
	virtual void NativeOnCurrentTextStyleChanged() override;
	
	// Chamado ao receber hover/foco.
	virtual void NativeOnHovered() override;
	
	// Chamado ao perder hover/foco.
	virtual void NativeOnUnhovered() override;
	//~ End UCommonButton Interface

	// ----------------------------------------------------------
	// Bound Widgets
	// ----------------------------------------------------------

	// TextBlock vinculado - exibe o texto do botão (opcional, permite botões sem texto)
	UPROPERTY(meta = (BindWidgetOptional))
	UCommonTextBlock* CommonTextBlock_ButtonText;

	// ----------------------------------------------------------
	// Properties
	// ----------------------------------------------------------
	
	// Texto padrão exibido no botão - configurável no painel Details do Blueprint filho.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = "true"))
	FText ButtonText;
	
	// Define se o texto exibido é convertido para maiúsculas.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = "true"))
	bool bUpperCaseText = false;
	
	// Descrição do botão - exibida como tooltip ou feedback ao hover via OnButtonDescriptionTextUpdate.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = "true"))
	FText ButtonDescriptionText;
};

// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "Widget_ConfirmScreen.generated.h"

class UCommonTextBlock;
class UDynamicEntryBox;

/**
* FConfirmScreenButtonInfo
*
* Struct que representa um botão do modal - armazena o tipo de retorno
* e o texto a ser exibido no botão.
*/
USTRUCT(BlueprintType)
struct FConfirmScreenButtonInfo
{
	GENERATED_BODY()
	
	// Tipo de resultado que este botão retorna ao ser clicado (Confirmed, Cancelled, Closed)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EConfirmScreenButtonType ConfirmScreenButtonType = EConfirmScreenButtonType::Unknown;
	
	// Texto exibido no botão
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ButtonTextToDisplay;
	
};


/**
* UConfirmScreenInfoObject
*
* Objeto de dados do modal - armazena o título, a mensagem e a lista de botões.
* Criado via Factory Functions estáticas para garantir configuração correta por tipo.
*/
UCLASS()
class FRONTENDUI_API UConfirmScreenInfoObject : public UObject
{
	GENERATED_BODY()
	
public:
	//? Factory function: Nome de função static que cria e configura objetos prontos ao invés de fazer manualmente.
	
	// Factory: cria pacote do modal Ok - 1 botão "Ok" que retorna Closed
	static UConfirmScreenInfoObject* CreateOkScreen(const FText& InScreenTitle, const FText& InScreenMsg);
	
	// Factory: cria pacote do modal Yes/No - 2 botões (Confirmed / Cancelled)
	static UConfirmScreenInfoObject* CreateYesNoScreen(const FText& InScreenTitle, const FText& InScreenMsg);
	
	// Factory: cria pacote do modal Ok/Cancel - 2 botões (Confirmed / Cancelled)
	static UConfirmScreenInfoObject* CreateOkCancelScreen(const FText& InScreenTitle, const FText& InScreenMsg);
	
	// Título exibido no topo do modal
	UPROPERTY(Transient)
	FText ScreenTitle;
	
    // Mensagem exibida no corpo do modal
	UPROPERTY(Transient)
	FText ScreenMessage;
	
    // Lista de botões disponíveis no modal - populada pelas Factory Functions
	UPROPERTY(Transient)
	TArray<FConfirmScreenButtonInfo> AvailableScreenButtons;
};

/**
* UWidget_ConfirmScreen
*
* Widget base do modal de confirmação do Frontend.
* Recebe um UConfirmScreenInfoObject com os dados e cria os botões dinamicamente via DynamicEntryBox.
* Cada botão dispara o callback com seu tipo (Confirmed, Cancelled, Closed) e fecha o modal.
*/
UCLASS(Abstract, BlueprintType, meta=(DisableNaiveTick))
class FRONTENDUI_API UWidget_ConfirmScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()
	
public:
	// Configura o modal: aplica título, mensagem e cria os botões com seus callbacks.
	void InitConfirmScreen(
		UConfirmScreenInfoObject* InScreenInfoObject,						// Pacote de Dados do modal (título/botões)
		TFunction<void(EConfirmScreenButtonType)> ClickedButtonCallback		// Callback disparado ao clicar
		);
	
	//~ Begin UCommonActivatableWidget Interface
	// Retorna o último botão criado como alvo de foco.
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	//~ End UCommonActivatableWidget Interface

private:
	// ----------------------------------------------------------
	// Bound Widgets
	// ----------------------------------------------------------
	
    // TextBlock vinculado - exibe o título do modal
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CommonTextBlock_Title;
	
    // TextBlock vinculado - exibe a mensagem do modal
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CommonTextBlock_Message;
	
    // EntryBox vinculado - container onde os botões são criados dinamicamente
	UPROPERTY(meta = (BindWidget))
	UDynamicEntryBox* DynamicEntryBox_Buttons;
};

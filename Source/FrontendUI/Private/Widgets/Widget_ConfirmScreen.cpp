// DvoraGames All Rights Reserved

#include "Widgets/Widget_ConfirmScreen.h"

#include "FrontendNamespacesHelper.h"

#include "CommonTextBlock.h"
#include "ICommonInputModule.h"
#include "Components/DynamicEntryBox.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"

using namespace FFrontendLocHelper;

// ---------------------------------------------------------
// UConfirmScreenInfoObject — Factory Functions
// ---------------------------------------------------------

// Essa função cria um "pacote de dados" para o modal OK
UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOkScreen(const FText& InScreenTitle, const FText& InScreenMsg)
{
	// Cria o objeto de dados vazio
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	
    // Preenche título e mensagem
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMsg;
	
    // Cria o botão Ok — retorna Closed ao clicar
	FConfirmScreenButtonInfo OkButtonInfo;										// Objeto Botao
	OkButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Closed;	// Retorno Botao
	OkButtonInfo.ButtonTextToDisplay = GetTableTextByKey("Generic.Ok");			// Nome Botao
	
	// Adiciona o botão à lista de botoes disponiveis no modal
	InfoObject->AvailableScreenButtons.Add(OkButtonInfo);
	
	// Retorna o pacote pronto
	return InfoObject;
}

// Essa função cria um "pacote de dados" para o modal Yes e NO.
UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateYesNoScreen(const FText& InScreenTitle,
	const FText& InScreenMsg)
{
	// Cria o objeto de dados vazio
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	
    // Preenche título e mensagem
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMsg;
	
    // Cria o botão Yes — retorna Confirmed ao clicar
	FConfirmScreenButtonInfo YesButtonInfo;
	YesButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
	YesButtonInfo.ButtonTextToDisplay = GetTableTextByKey("Generic.Yes");
	
    // Cria o botão No — retorna Cancelled ao clicar
	FConfirmScreenButtonInfo NoButtonInfo;
	NoButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
	NoButtonInfo.ButtonTextToDisplay = GetTableTextByKey("Generic.No");
	
	// Adiciona o botão à lista de botoes disponiveis no modal
	InfoObject->AvailableScreenButtons.Add(YesButtonInfo);
	InfoObject->AvailableScreenButtons.Add(NoButtonInfo);
	
	// Retorna o pacote pronto
	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOkCancelScreen(const FText& InScreenTitle,
	const FText& InScreenMsg)
{
	// Cria o objeto de dados vazio
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	
    // Preenche título e mensagem
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMsg;
	
    // Cria o botão Ok — retorna Confirmed ao clicar
	FConfirmScreenButtonInfo OkButtonInfo;	
	OkButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
	OkButtonInfo.ButtonTextToDisplay = GetTableTextByKey("Generic.Ok");
	
    // Cria o botão Cancel — retorna Cancelled ao clicar
	FConfirmScreenButtonInfo CancelButtonInfo;
	CancelButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
	CancelButtonInfo.ButtonTextToDisplay = GetTableTextByKey("Generic.Cancel");
	
	// Adiciona o botão à lista de botoes disponiveis no modal
	InfoObject->AvailableScreenButtons.Add(OkButtonInfo);
	InfoObject->AvailableScreenButtons.Add(CancelButtonInfo);
	
	// Retorna o pacote pronto
	return InfoObject;
}

// ---------------------------------------------------------
// UWidget_ConfirmScreen
// ---------------------------------------------------------

void UWidget_ConfirmScreen::InitConfirmScreen(UConfirmScreenInfoObject* InScreenInfoObject,
	TFunction<void(EConfirmScreenButtonType)> ClickedButtonCallback)
{
	// Garante que todos os BindWidgets e o pacote de dados são válidos
	check(InScreenInfoObject && CommonTextBlock_Title && CommonTextBlock_Message && DynamicEntryBox_Buttons);
	
	// Aplica título e mensagem nos TextBlocks vinculados
	CommonTextBlock_Title->SetText(InScreenInfoObject->ScreenTitle);
	CommonTextBlock_Message->SetText(InScreenInfoObject->ScreenMessage);
	
	// Limpa botões antigos antes de recriar — evita duplicatas ao reutilizar o modal
	if (DynamicEntryBox_Buttons->GetNumEntries() != 0)
	{
		// Itera sobre cada botão existente e desconecta os callbacks antes de destruí-los
		DynamicEntryBox_Buttons->Reset<UFrontendCommonButtonBase>(
			[](UFrontendCommonButtonBase& ExistingButton)
			{
				// Desconecta todos os lambdas vinculados ao OnClicked deste botão
				ExistingButton.OnClicked().Clear();
			}
			);
	}
	
    // Garante que há pelo menos um botão para criar
	check(!InScreenInfoObject->AvailableScreenButtons.IsEmpty());
	
	// Cria 1 botão por cada FConfirmScreenButtonInfo no array
	for (const FConfirmScreenButtonInfo& ButtonInfo : InScreenInfoObject->AvailableScreenButtons)
	{
		// Handle do InputAction que será vinculado ao botão (Enhanced Input)
		UInputAction* InputActionHandle = nullptr;
		
		// Define o InputAction correto baseado no tipo do botão
		switch (ButtonInfo.ConfirmScreenButtonType)
		{
		case EConfirmScreenButtonType::Cancelled:
		case EConfirmScreenButtonType::Closed:
			// Botões de cancelar/fechar usam o BackAction — ex: Gamepad B / Teclado ESC
			InputActionHandle = ICommonInputModule::GetSettings().GetEnhancedInputBackAction();
			break;
		default:
			break;
		}
		
		// Cria o botão dinamicamente no EntryBox
		UFrontendCommonButtonBase* AddedButton = DynamicEntryBox_Buttons->CreateEntry<UFrontendCommonButtonBase>();
		
		// Aplica o texto no botão criado
		AddedButton->SetButtonText(ButtonInfo.ButtonTextToDisplay);
        // Aplica o input action no botão criado
		AddedButton->SetTriggeringEnhancedInputAction(InputActionHandle);
		
		// Lambda que executa ao clicar: dispara callback, fecha o modal e restaura o foco
		AddedButton->OnClicked().AddLambda(
			[ClickedButtonCallback, ButtonInfo, this]()
			{
				// Retorna o tipo do botão clicado via callback
				ClickedButtonCallback(ButtonInfo.ConfirmScreenButtonType);
				
				// Fecha o modal removendo-o do stack
				DeactivateWidget();
			});
	}
}

UWidget* UWidget_ConfirmScreen::NativeGetDesiredFocusTarget() const
{
	// Verica se existem botões na EntryBox
	if (DynamicEntryBox_Buttons->GetNumEntries() != 0)
	{
		// Foca no último botão criado.
		return DynamicEntryBox_Buttons->GetAllEntries().Last();
	}
	
	return Super::NativeGetDesiredFocusTarget();
}

// DvoraGames All Rights Reserved

#include "Widgets/Widget_ConfirmScreen.h"
#include "CommonTextBlock.h"
#include "ICommonInputModule.h"
#include "Components/DynamicEntryBox.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"

// Essa função cria um "pacote de dados" para o modal OK.
UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOkScreen(const FText& InScreenTitle, const FText& InScreenMsg)
{
	// Cria um objeto vazio
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	
	// Preenche título e mensagem do modal
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMsg;
	
	// Cria o botão do OK usando a Struct
	FConfirmScreenButtonInfo OkButtonInfo;
	// Aplica o que esse botão irá retornar ao clicar nele (Closed)
	OkButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Closed;
	// Altera o texto do Botao para OK
	OkButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("Ok"));
	
	// Adiciona o botão na lista de botoes disponiveis no modal
	InfoObject->AvailableScreenButtons.Add(OkButtonInfo);
	
	// Retorna o pacote pronto
	return InfoObject;
}

// Essa função cria um "pacote de dados" para o modal Yes e NO.
UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateYesNoScreen(const FText& InScreenTitle,
	const FText& InScreenMsg)
{
	// Cria um objeto vazio
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	
	// Preenche título e mensagem do modal
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMsg;
	
	// Cria o botão Yes usando a Struct
	FConfirmScreenButtonInfo YesButtonInfo;
	// Aplica o que esse botão irá retornar ao clicar nele (Confirmed)
	YesButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
	// Altera o texto do Botao para Yes
	YesButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("Yes"));
	
	// Cria o botão No usando a Struct
	FConfirmScreenButtonInfo NoButtonInfo;
	// Aplica o que esse botão irá retornar ao clicar nele (Cancelled)
	NoButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
	// Altera o texto do Botao para No
	NoButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("No"));
	
	// Adiciona os botões na lista de botoes disponiveis no modal
	InfoObject->AvailableScreenButtons.Add(YesButtonInfo);
	InfoObject->AvailableScreenButtons.Add(NoButtonInfo);
	
	// Retorna o pacote pronto
	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOkCancelScreen(const FText& InScreenTitle,
	const FText& InScreenMsg)
{
	// Cria um objeto vazio
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	
	// Preenche título e mensagem do modal
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMsg;
	
	// Cria o botão Ok usando a Struct
	FConfirmScreenButtonInfo OkButtonInfo;	
	// Aplica o que esse botão irá retornar ao clicar nele (Confirmed)
	OkButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
	// Altera o texto do Botao para Ok
	OkButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("Ok"));
	
	// Cria o botão Cancel usando a Struct
	FConfirmScreenButtonInfo CancelButtonInfo;
	// Aplica o que esse botão irá retornar ao clicar nele (Cancelled)
	CancelButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
	// Altera o texto do Botao para Cancel
	CancelButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("Cancel"));
	
	// Adiciona os botões na lista de botoes disponiveis no modal
	InfoObject->AvailableScreenButtons.Add(OkButtonInfo);
	InfoObject->AvailableScreenButtons.Add(CancelButtonInfo);
	
	// Retorna o pacote pronto
	return InfoObject;
}

// Função responsavel por configurar o modal: aplica dados e conecta callbacks dos botões
void UWidget_ConfirmScreen::InitConfirmScreen(UConfirmScreenInfoObject* InScreenInfoObject,
	TFunction<void(EConfirmScreenButtonType)> ClickedButtonCallback)
{
	// Verifica se todas as propriedades existem (crash se tiver alguma nula)
	check(InScreenInfoObject && CommonTextBlock_Title && CommonTextBlock_Message && DynamicEntryBox_Buttons);
	
	// Aplica o título e mensagem nos TextBlocks bindados
	CommonTextBlock_Title->SetText(InScreenInfoObject->ScreenTitle);
	CommonTextBlock_Message->SetText(InScreenInfoObject->ScreenMessage);
	
	// Verifica se existem botões antigos para limpar antes de criar novos
	if (DynamicEntryBox_Buttons->GetNumEntries() != 0)
	{
		// Reset limpa + remove callbacks antigos
		DynamicEntryBox_Buttons->Reset<UFrontendCommonButtonBase>(
			[](UFrontendCommonButtonBase& ExistingButton)
			{
				ExistingButton.OnClicked().Clear(); // Desconecta lambdas velhos (callbacks)
			}
			);
	}
	
	// Verifica se a lista de botões etá vazia, se não tiver crasha
	check(!InScreenInfoObject->AvailableScreenButtons.IsEmpty());
	
	//Cria 1 botão por cada FConfirmScreenButtonInfo dentro do array de botões
	for (const FConfirmScreenButtonInfo& ButtonInfo : InScreenInfoObject->AvailableScreenButtons)
	{
		// Mapeia a Tecla/Botão do Hardware (ClickAction e BackAction) para o botão do modal
		FDataTableRowHandle InputActionRowHandle;
		
		// Mapeia a Tecla/Botão do Hardware (ClickAction e BackAction) para o botão do modal usando o EnhancedInput
		UInputAction* InputActionHandle = nullptr;
		
		// Input para cada tipo de botão do modal
		switch (ButtonInfo.ConfirmScreenButtonType)
		{
		case EConfirmScreenButtonType::Cancelled:
			// Se o botão modal for Cancelled, usa o Input BackAction (Ex. Gamepad: Face Right (B/Bolinha))
			/* Permite usar o Input BackAction da DataTable para fazer a mesma função do botão do tipo Cancelled do modal */
			InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultBackAction();
			
			/* Permite usar o InputAction (IA_UI_Back) para fazer a mesma função do botão do tipo Cancelled do modal */
			InputActionHandle = ICommonInputModule::GetSettings().GetEnhancedInputBackAction();
			break;
		case EConfirmScreenButtonType::Closed:
			// Se o botão modal for Closed, usa o Input BackAction (Ex. Gamepad: Face Right (B/Bolinha))
			InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultBackAction();
			
			/* Permite usar o InputAction (IA_UI_Back) para fazer a mesma função do botão do tipo Closed do modal */
			InputActionHandle = ICommonInputModule::GetSettings().GetEnhancedInputBackAction();
			break;
		default:
			break;
		}
		
		// Cria botão dinamicamente no EntryBox
		UFrontendCommonButtonBase* AddedButton = DynamicEntryBox_Buttons->CreateEntry<UFrontendCommonButtonBase>();
		// Aplica o texto do botão atual do loop com o Texto que foi setado anteriormente nele
		AddedButton->SetButtonText(ButtonInfo.ButtonTextToDisplay);
		// Aplica o Enhanced Input Action do botão atual do loop com o Input Action que foi setado acima
		AddedButton->SetTriggeringEnhancedInputAction(InputActionHandle);
		
		// Lambda responsavel por adicionar a logica de Click do botão
		AddedButton->OnClicked().AddLambda(
			[ClickedButtonCallback, ButtonInfo, this]()
			{
				// Chama o callback do subsystem e passa o tipo do botão como resultado
				ClickedButtonCallback(ButtonInfo.ConfirmScreenButtonType);
				
				// Fecha modal
				DeactivateWidget();
			});
	}
}

// Sobrescreve foco CommonUI: sempre último botão
// Fix Gamepad→Mouse→Gamepad: restaura foco modal após click fora
UWidget* UWidget_ConfirmScreen::NativeGetDesiredFocusTarget() const
{
	// Verica se a quantidade de botões é diferente de 0
	if (DynamicEntryBox_Buttons->GetNumEntries() != 0)
	{
		// Foca no ÚLTIMO botão criado
		return DynamicEntryBox_Buttons->GetAllEntries().Last();
	}
	
	return Super::NativeGetDesiredFocusTarget();
}

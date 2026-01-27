// DvoraGames All Rights Reserved

#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "CommonTextBlock.h"
#include "Subsystems/FrontendUISubsystem.h"

// Função usada para aplicar o texto ao botão
void UFrontendCommonButtonBase::SetButtonText(FText InText)
{
	// Verifica se o BindWidget existe e se InText não está vazio
	if (CommonTextBlock_ButtonText && !InText.IsEmpty())
	{
		// Aplica uppercase se configurado (bUpperCaseText), senão usa texto original
		CommonTextBlock_ButtonText->SetText(bUpperCaseText? InText.ToUpper() : InText);
	}
}

void UFrontendCommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	// Define texto inicial usando valor padrão da propriedade ButtonText
	SetButtonText(ButtonText);
}

// Função chamada quando o estado do texto do Botão muda (Normal, Hover, Selected e Disable)
void UFrontendCommonButtonBase::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();
	
	// Verifica se o BindWidget CommonTextBlock_ButtonText é valido e se o Text Style não está vazio
	if (CommonTextBlock_ButtonText && GetCurrentTextStyleClass())
	{
		// Se for, pega a classe do style do Texto definido no botão e aplica ao botão
		CommonTextBlock_ButtonText->SetStyle(GetCurrentTextStyleClass());
	}
}

// Override chamado quando este botão recebe hover/foco.
void UFrontendCommonButtonBase::NativeOnHovered()
{
	Super::NativeOnHovered();
	
	// Verifica se ButtonDescriptionText não está vazio, sem texto
	if (!ButtonDescriptionText.IsEmpty())
	{
		// Se tiver texto, dispara notificação global para atualizar a descrição do botão focado.
		UFrontendUISubsystem::GetFrontendSubsystem(this)->OnButtonDescriptionTextUpdate.Broadcast(
			this,
			ButtonDescriptionText);
	}
}

// Override chamado quando este botão perde hover/foco.
void UFrontendCommonButtonBase::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();
	
	// Dispara notificação global para limpar a descrição dos botões.
	UFrontendUISubsystem::GetFrontendSubsystem(this)->OnButtonDescriptionTextUpdate.Broadcast(
			this,
			FText::GetEmpty());
}

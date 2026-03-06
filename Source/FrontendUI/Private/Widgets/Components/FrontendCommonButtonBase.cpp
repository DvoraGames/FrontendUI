// DvoraGames All Rights Reserved

#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "CommonTextBlock.h"
#include "Subsystems/FrontendUISubsystem.h"

// Função usada para aplicar o texto ao botão
void UFrontendCommonButtonBase::SetButtonText(FText InText)
{
	// Só aplica se o BindWidget existe e o texto não está vazio
	if (CommonTextBlock_ButtonText && !InText.IsEmpty())
	{
		// Aplica uppercase se configurado, senão usa o texto original
		CommonTextBlock_ButtonText->SetText(bUpperCaseText? InText.ToUpper() : InText);
	}
}

FText UFrontendCommonButtonBase::GetButtonDisplayText() const
{
	// Se o BindWidget for valido
	if (CommonTextBlock_ButtonText)
	{
		// Retorna o texto atualmente exibido no TextBlock vinculado
		return CommonTextBlock_ButtonText->GetText();
	}
	
	// Retorna FText vazio se o BindWidget não estiver disponível
	return FText();
}

void UFrontendCommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	// Aplica o valor padrão de ButtonText ao TextBlock antes de renderizar
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

void UFrontendCommonButtonBase::NativeOnHovered()
{
	Super::NativeOnHovered();
	
	// Só dispara se houver descrição configurada, evitando broadcast desnecessário
	if (!ButtonDescriptionText.IsEmpty())
	{
		// Notifica o subsystem para exibir a descrição deste botão em todos os listeners
		UFrontendUISubsystem::Get(this)->OnButtonDescriptionTextUpdate.Broadcast(
			this,
			ButtonDescriptionText);
	}
}

void UFrontendCommonButtonBase::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();
	
	// Limpa a descrição exibida notificando o subsystem com texto vazio
	UFrontendUISubsystem::Get(this)->OnButtonDescriptionTextUpdate.Broadcast(
			this,
			FText::GetEmpty());
}

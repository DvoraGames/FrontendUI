// DvoraGames All Rights Reserved


#include "Widgets/Options/Widget_OptionsDetailsView.h"

#include "CommonLazyImage.h"
#include "CommonRichTextBlock.h"
#include "CommonTextBlock.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"

void UWidget_OptionsDetailsView::UpdateDetailsViewInfo(UListDataObject_Base* InDataObject,
                                                       const FString& InEntryClassName)
{
	// Ignora chamadas sem DataObject válido.
	if (!InDataObject)
	{
		return;
	}
	
	// Atualiza o título com o nome da opção.
	CommonTextBlock_Title->SetText(InDataObject->GetDataDisplayName());
	
	// Verifica se a imagem não é nula
	if (!InDataObject->GetSoftDescriptionImage().IsNull())
	{
		// Atualiza a imagem setada no DataObject ao Widget Details View
		CommonLazyImage_DescriptionImage->SetBrushFromLazyTexture(InDataObject->GetSoftDescriptionImage());
		
		// Define a visibilidade como visivel, mas sem interação
		CommonLazyImage_DescriptionImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	
	// Atualiza a Descrição com o texto que foi setado no Data Registry.
	CommonRichTextBlock_Description->SetText(InDataObject->GetDescriptionRichText());
	
	// Gera os Detalhes Dinamicos usando o nome da Classe do DataObject eo nome da classe do Widget do Item
	const FString DynamicDetails = FString::Printf(
		TEXT("Data Object Class: <Bold>%s</>\n\nEntry Widget Class: <Bold>%s</>"), 
		*InDataObject->GetClass()->GetName(), 
		*InEntryClassName);
	
	// Atualiza os Detalhes Dinamicos com o texto que foi setado acima.
	CommonRichTextBlock_DynamicDetails->SetText(FText::FromString(DynamicDetails));
	
	// Atualiza o texto do motivo da opção estar desabilitada
	CommonRichTextBlock_DisabledReason->SetText(InDataObject->GetDisabledRichText());
}

void UWidget_OptionsDetailsView::ClearDetailsViewInfo() const
{
	// Limpa o Texto do Title
	CommonTextBlock_Title->SetText(FText::GetEmpty());
	// Esconde a imagem colapsando-a
	CommonLazyImage_DescriptionImage->SetVisibility(ESlateVisibility::Collapsed);
	// Limpa o texto da Descrição
	CommonRichTextBlock_Description->SetText(FText::GetEmpty());
	// Limpa o texto dos detalhes dinamicos
	CommonRichTextBlock_DynamicDetails->SetText(FText::GetEmpty());
	// Limpa o texto da razão da opção estar desabilitada
	CommonRichTextBlock_DisabledReason->SetText(FText::GetEmpty());
}

void UWidget_OptionsDetailsView::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	// Garante que o painel comece limpo antes de receber qualquer dado da OptionsScreen.
	ClearDetailsViewInfo();
}

// DvoraGames All Rights Reserved


#include "Widgets/Options/Widget_OptionsDetailsView.h"

#include "CommonLazyImage.h"
#include "CommonRichTextBlock.h"
#include "CommonTextBlock.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"

void UWidget_OptionsDetailsView::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	// Garante que o painel comece limpo antes de receber qualquer dado
	ClearDetailsViewInfo();
}

void UWidget_OptionsDetailsView::UpdateDetailsViewInfo(UListDataObject_Base* InDataObject,
                                                       const FString& InEntryClassName)
{
    // Ignora chamadas sem DataObject válido
	if (!InDataObject) return;
	
    // Atualiza o título com o nome da opção
	CommonTextBlock_Title->SetText(InDataObject->GetDataDisplayName());
	
	// Verifica se a imagem está configurada no DataObject
	if (!InDataObject->GetSoftDescriptionImage().IsNull())
	{
		// Aplica a imagem de forma lazy (assíncrona) ao widget
		CommonLazyImage_DescriptionImage->SetBrushFromLazyTexture(InDataObject->GetSoftDescriptionImage());
		
		// Torna a imagem visível, mas sem capturar interações de input
		CommonLazyImage_DescriptionImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		// Colapsa a imagem se não houver nenhuma configurada
		CommonLazyImage_DescriptionImage->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	// Atualiza a descrição estática da opção com o texto do DataObject.
	CommonRichTextBlock_Description->SetText(InDataObject->GetDescriptionRichText());
	
	// Gera os detalhes dinâmicos com o nome da classe do DataObject e do Entry Widget
	const FString DynamicDetails = FString::Printf(
		TEXT("Data Object Class: <Bold>%s</>\n\nEntry Widget Class: <Bold>%s</>"), 
		*InDataObject->GetClass()->GetName(), 
		*InEntryClassName);
	
	// Aplica os detalhes dinâmicos gerados acima
	CommonRichTextBlock_DynamicDetails->SetText(FText::FromString(DynamicDetails));
	
    // Atualiza o motivo de desabilitação da opção
	CommonRichTextBlock_DisabledReason->SetText(InDataObject->GetDisabledRichText());
}

void UWidget_OptionsDetailsView::ClearDetailsViewInfo() const
{
    // Limpa o título
	CommonTextBlock_Title->SetText(FText::GetEmpty());
	
    // Colapsa a imagem
	CommonLazyImage_DescriptionImage->SetVisibility(ESlateVisibility::Collapsed);
	
    // Limpa a descrição estática
	CommonRichTextBlock_Description->SetText(FText::GetEmpty());
	
	// Limpa os detalhes dinamicos
	CommonRichTextBlock_DynamicDetails->SetText(FText::GetEmpty());
	
    // Limpa o motivo de desabilitação
	CommonRichTextBlock_DisabledReason->SetText(FText::GetEmpty());
}

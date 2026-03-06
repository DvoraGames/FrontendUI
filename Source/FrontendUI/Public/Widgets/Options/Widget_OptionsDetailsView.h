// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_OptionsDetailsView.generated.h"

class UListDataObject_Base;
class UCommonRichTextBlock;
class UCommonLazyImage;
class UCommonTextBlock;

/**
* UWidget_OptionsDetailsView
*
* Painel de detalhes das opções de configuração.
* Recebe um UListDataObject_Base e exibe seus metadados informativos:
* título, imagem, descrição, detalhes dinâmicos e motivo de desabilitação.
*/
UCLASS(Abstract, BlueprintType, meta=(DisableNaiveTick))
class FRONTENDUI_API UWidget_OptionsDetailsView : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Popula os Bound Widgets com as informações do DataObject - chamado pela OptionsScreen ao hover ou seleção de um item.
	void UpdateDetailsViewInfo(UListDataObject_Base* InDataObject, const FString& InEntryClassName = FString());
	
	// Limpa todos os campos do painel - chamado ao trocar de aba ou quando nenhum item está hoverado/selecionado.
	void ClearDetailsViewInfo() const;
	
protected:
	//~ Begin UUserWidget Interface
	// Garante que o painel comece limpo antes de receber qualquer dado da OptionsScreen.
	virtual void NativeOnInitialized() override;
	//~ End UUserWidget Interface
	
private:
	// ----------------------------------------------------------
	// Bound Widgets
	// ----------------------------------------------------------
	
	// Título da opção atualmente em foco.
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CommonTextBlock_Title;
	
	// Imagem ilustrativa da opção - carregada de forma lazy (assíncrona).
	UPROPERTY(meta = (BindWidget))
	UCommonLazyImage* CommonLazyImage_DescriptionImage;
	
	// Descrição estática da opção em RichText - suporta formatação avançada (negrito, cor, ícones inline).
	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonRichTextBlock_Description;
	
	// Detalhes dinâmicos da opção em RichText - atualizado com informações contextuais em tempo real.
	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonRichTextBlock_DynamicDetails;
	
	// Motivo pelo qual a opção está desabilitada em RichText - exibido quando o DataObject indica bloqueio
	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonRichTextBlock_DisabledReason;
};

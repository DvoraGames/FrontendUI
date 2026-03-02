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
 * Painel de detalhes das opções de configuração.
 * Recebe um UListDataObject_Base e exibe seus metadados informativos:
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNaiveTick))
class FRONTENDUI_API UWidget_OptionsDetailsView : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Popula os Bound widgets com as informações do DataObject fornecido.
	/* Chamado pela OptionsScreen ao detectar hover ou seleção de um item. */
	void UpdateDetailsViewInfo(UListDataObject_Base* InDataObject, const FString& InEntryClassName = FString());
	
	// Limpa todos os campos do painel de detalhes.
	/* Chamado quando nenhum item está hovereado/selecionado, ou ao trocar de aba na OptionsScreen. */
	void ClearDetailsViewInfo() const;
	
protected:
	//~ Begin UUserWidget Interface
	// Função chamada na Inicialização do Widget. Usado para configurar estados iniciais dos campos
	virtual void NativeOnInitialized() override;
	//~ End UUserWidget Interface
	
private:
	/***** Bound Widgets *****/
	// Título da opção atualmente em foco.
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CommonTextBlock_Title;
	
	// Imagem ilustrativa da opção, carregada de forma lazy (assíncrona).
	UPROPERTY(meta = (BindWidget))
	UCommonLazyImage* CommonLazyImage_DescriptionImage;
	
	// Descrição estática da opção em RichText. Suporta formatação avançada (negrito, cor, ícones inline).
	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonRichTextBlock_Description;
	
	// Detalhes dinâmicos da opção em RichText. Atualizado com informações contextuais em tempo real.
	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonRichTextBlock_DynamicDetails;
	
	/* Motivo pelo qual a opção está desabilitada, em RichText. Exibido apenas quando o DataObject indica que a opção 
	está bloqueada (ex: "Requer reinicialização do jogo"). */
	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonRichTextBlock_DisabledReason;
	/***** Bound Widgets *****/
};

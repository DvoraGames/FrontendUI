// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "Widget_ConfirmScreen.generated.h"

class UCommonTextBlock;
class UDynamicEntryBox;

// Struct Blueprint de cada Botão do modal
USTRUCT(BlueprintType)
struct FConfirmScreenButtonInfo
{
	GENERATED_BODY()
	
	// Variavel responsavel pelo tipo de resultado que esse botão retorna
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EConfirmScreenButtonType ConfirmScreenButtonType = EConfirmScreenButtonType::Unknown;
	
	// Variavel responsavel pelo texto exibido no botão
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ButtonTextToDisplay;
	
};

// Classe de dados para o modal. Armazena configuração completa do modal.
UCLASS()
class FRONTENDUI_API UConfirmScreenInfoObject : public UObject
{
	GENERATED_BODY()
	
public:
	/* Factory function = É um nome para função static que cria e configura objetos prontos ao invés de fazer manualmente. */
	
    // Factory: Cria pacote OK (1 botão "Ok" → Closed)
	static UConfirmScreenInfoObject* CreateOkScreen(const FText& InScreenTitle, const FText& InScreenMsg);
    // Factory: Cria pacote Yes/No (2 botões)
	static UConfirmScreenInfoObject* CreateYesNoScreen(const FText& InScreenTitle, const FText& InScreenMsg);
    // Factory: Cria pacote Ok/Cancelar (2 botões)
	static UConfirmScreenInfoObject* CreateOkCancelScreen(const FText& InScreenTitle, const FText& InScreenMsg);
	
	// Variavel do titulo do modal 
	UPROPERTY(Transient)
	FText ScreenTitle;
	
	// Variavel da mensagem do modal 
	UPROPERTY(Transient)
	FText ScreenMessage;
	
	// Lista dos Botões do modal
	UPROPERTY(Transient)
	TArray<FConfirmScreenButtonInfo> AvailableScreenButtons;
};

/**
 * 
 */
// Classe BASE pro modal (herdar no BP, NÃO instanciar direto)
// Abstract: só pra herdar
// BlueprintType: BP class child 
// DisableNaiveTick: filhos BP não tickam
UCLASS(Abstract, BlueprintType, meta=(DisableNaiveTick))
class FRONTENDUI_API UWidget_ConfirmScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()
	
public:
	// Função responsavel por configurar o modal: aplica dados e conecta callbacks dos botões
	void InitConfirmScreen(
		UConfirmScreenInfoObject* InScreenInfoObject,						// Dados do modal (título/botões)
		TFunction<void(EConfirmScreenButtonType)> ClickedButtonCallback		// Função que roda no clique
		);
	
	//~ Begin UCommonActivatableWidget Interface
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	//~ End UCommonActivatableWidget Interface

private:
	// Variavel BindWidget do Titulo do modal
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CommonTextBlock_Title;
	
	// Variavel BindWidget da Mensagem do modal
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CommonTextBlock_Message;
	
	// Variavel BindWidget dos Botões do modal
	UPROPERTY(meta = (BindWidget))
	UDynamicEntryBox* DynamicEntryBox_Buttons;
};

// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "FrontendUISubsystem.generated.h"

class UFrontendCommonButtonBase;
class UWidget_PrimaryLayout;
struct FGameplayTag;
class UWidget_ActivatableBase;

// Enum que define os dois estágios do callback de push de widget
enum class EAsyncPushWidgetState : uint8
{
	OnCreatedBeforePush,	// Widget criado, ainda NÃO inserido no stack
	AfterPush				// Widget inserido no stack, ativado e visível
};

// Delegate disparado para notificar listeners sobre a atualização da descrição de um botão
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnButtonDescriptionTextUpdateDelegate,			// Nome do delegate
	UFrontendCommonButtonBase*,	BroadcastingButton,	// Botão que disparou o evento (Tipo e Nome do parametro)
	FText, Description								// Texto de descrição do botão (Tipo e Nome do parametro)
	);

/**
* UFrontendUISubsystem
*
* Subsystem central de UI do Frontend — roda na GameInstance durante toda a sessão do jogo.
* Responsável por gerenciar o PrimaryLayout, fazer push de widgets nos stacks via AsyncLoad,
* exibir modais de confirmação e gerenciar o Input Mapping Context global da UI.
*
* Não é criado em servidores dedicados nem se existir uma subclasse registrada.
*/
UCLASS()
class FRONTENDUI_API UFrontendUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// Retorna a instância ativa do subsystem a partir de qualquer UObject com contexto de World.
	static UFrontendUISubsystem* Get(const UObject* WorldContextObject);
	
	//~ Begin USubsystem Interface
	// Determina se este subsystem deve ser criado.
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	//~ End USubsystem Interface
	
	// Registra o PrimaryLayout criado para que o subsystem possa gerenciar os stacks.
	UFUNCTION(BlueprintCallable)
	void RegisterCreatedPrimaryLayoutWidget(UWidget_PrimaryLayout* InCreatedWidget);
	
	// Carrega assincronamente uma classe de Widget e a adiciona ao stack especificado pela GameplayTag. 
	void PushSoftWidgetToStackAsync(const FGameplayTag& InWidgetStackTag, 
		TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass, 
		TFunction<void(EAsyncPushWidgetState, UWidget_ActivatableBase*)> AsyncPushStateCallback);
	
	// FUNÇÃO PRINCIPAL: Carrega e exibe assincronamente um modal de confirmação no stack Modal.
	/* Etapas:
	 * 1. Cria pacote UConfirmScreenInfoObject baseado no tipo
	 * 2. Chama PushSoftWidgetToStackAsync com a classe do ConfirmScreen
	 * 3. O widget chama InitConfirmScreen automaticamente no OnCreatedBeforePush
	 * 4. O callback é chamado com o resultado quando o usuário clica em um botão 
	 */
	void PushConfirmScreenToModalStackAsync(
		EConfirmScreenType InScreenType,									// Tipo do Modal (Ok/YesNo/OkCancel)
		const FText& InScreenTitle,											// Título do exibido no modal
		const FText& InScreenMsg,											// Mensagem exibida no modal
		TFunction<void (EConfirmScreenButtonType)> ButtonClickedCallback	// Callback com o tipo do botão clicado
		);
	
	// Delegate disparado quando a descrição de um botão é atualizada - usado para exibir tooltips/feedback.
	UPROPERTY(BlueprintAssignable)
	FOnButtonDescriptionTextUpdateDelegate OnButtonDescriptionTextUpdate;
	
	// Função para adicionar o Input Mapping Context da UI ao Enhanced Input via BP
	UFUNCTION(BlueprintCallable)
	static void AddGlobalInputMappingContext(AFrontendPlayerController* FrontendPC, int32 Priority);
	
	// Função para remover o Input Mapping Context da UI do Enhanced Input via BP.
	UFUNCTION(BlueprintCallable)
	void RemoveGlobalInputMappingContext(AFrontendPlayerController* FrontendPC);

private:
	// Referência ao PrimaryLayout ativo - ponto raiz de todos os stacks de widgets.
	UPROPERTY(Transient)
	UWidget_PrimaryLayout* CreatedPrimaryLayout;
};
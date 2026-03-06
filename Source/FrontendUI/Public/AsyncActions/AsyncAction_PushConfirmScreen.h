// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_PushConfirmScreen.generated.h"

/**
* UAsyncActionPushConfirmScreen
*
* AsyncAction que exibe assincronamente um modal de confirmação no stack Modal.
* Internamente delega ao FrontendUISubsystem a criação do pacote de dados e o push do widget.
*
* Expõe um pino de execução no Blueprint:
* - OnButtonClicked: disparado quando o usuário clica em qualquer botão do modal — retorna o tipo do botão clicado.
*/

// Delegate multicast compatível com Blueprint (dynamic) - disparado com o tipo do botão clicado como parâmetro
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnConfirmScreenButtonClickedDelegate,	// Nome do Delegate
	EConfirmScreenButtonType,				// Tipo do parametro
	ClickedButtonType						// Nome do Parametro
	);

UCLASS()
class FRONTENDUI_API UAsyncAction_PushConfirmScreen : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, meta=(
		WorldContext = "WorldContextObject",  // Indica qual parâmetro fornece o contexto do UWorld
		HidePin = "WorldContextObject",  // Oculta o pin WorldContextObject no BP, pois ele é inferido automaticamente pelo nó.
		BlueprintInternalUseOnly = "true"),  // Cria o nó assíncrono com pinos de execução de saída
		DisplayName="Push Confirmation Screen")  // Nome amigável que aparece no Blueprint.
	static UAsyncAction_PushConfirmScreen* PushConfirmScreen(
		const UObject* WorldContextObject,  // Objeto que fornece contexto do mundo, geralmente auto-preenchido no Blueprint.
		EConfirmScreenType ConfirmScreenType,  // Tipo de modal a ser exibido (Ok, YesNo ou OkCancel)
		FText InScreenTitle,  // Título exibido no topo da tela de confirmação
		FText InScreenMessage  // Mensagem principal exibida no corpo da tela de confirmação

		);
	
	//~ Begin UBlueprintAsyncActionBase Interface
	// Gatilho da AsyncAction — chamado quando o nó é executado no Blueprint, iniciando a lógica assíncrona.
	virtual void Activate() override;
	//~ End UBlueprintAsyncActionBase Interface
	
	// ----------------------------------------------------------
	// Delegates
	// ----------------------------------------------------------
	
	// Disparado quando o usuário clica em um botão do modal - retorna o tipo do botão clicado.
	UPROPERTY(BlueprintAssignable)
	FOnConfirmScreenButtonClickedDelegate OnButtonClicked;
	
private:
	// ----------------------------------------------------------
	// Cache
	// ----------------------------------------------------------
	
	// Cache fraco do World - evita dangling pointer durante o carregamento assíncrono.
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	
	// Cache do tipo de modal (Ok, YesNo, OkCancel) - usado no Activate() para montar o pacote.
	EConfirmScreenType CachedScreenType;
	
	// Cache do título do modal - mantido até a tela ser criada.
	FText CachedScreenTitle;
	
	// Cache da mensagem do modal - mantido até a tela ser criada.
	FText CachedScreenMessage;
};

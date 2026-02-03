// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_PushConfirmScreen.generated.h"

/**
 * Define um delegate dinâmico multicast (compatível com Blueprint).
 * Dynamic: Suporta serialização e reflexão, permitindo uso em Blueprints.
 * Multicast: Permite múltiplas funções vinculadas que serão executadas simultaneamente.
 * 
 * Este delegate é disparado quando um botão do ConfirmScreen é clicado.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnConfirmScreenButtonClickedDelegate,	// Nome do Delegate
	EConfirmScreenButtonType,				// Tipo do parametro
	ClickedButtonType						// Nome do Parametro
	);

/**
 * 
 */
UCLASS()
class FRONTENDUI_API UAsyncAction_PushConfirmScreen : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, meta=(
		// Indica qual parâmetro fornece o contexto do mundo (UWorld*), necessário para operações que dependem do gameplay ativo.
		WorldContext = "WorldContextObject",
		// Oculta o pin do WorldContextObject no Blueprint, já que ele é inferido automaticamente pelo nó.
		HidePin = "WorldContextObject",
		// Marca que essa função cria um nó assíncrono especial (latent node com pinos de execução de saída).
		BlueprintInternalUseOnly = "true"),
		// Nome amigável que aparece no Blueprint em vez do nome real da função.
		DisplayName="Push Confirmation Screen")
	static UAsyncAction_PushConfirmScreen* PushConfirmScreen(
		// Objeto que fornece contexto do mundo, geralmente auto-preenchido no Blueprint.
		const UObject* WorldContextObject, 
		// Tipo de modal a ser exibido: Ok, YesNo ou OkCancel
		EConfirmScreenType ConfirmScreenType,
		// Título exibido no topo da tela de confirmação
		FText InScreenTitle,
		// Mensagem principal exibida no corpo da tela de confirmação
		FText InScreenMessage
		);
	
	//~ Begin UBlueprintAsyncActionBase Interface
	/* Sobrescreve a função Activate() que é o "gatilho" das AsyncActions do Blueprint. 
	É chamado quando o nó é executado no BP, iniciando sua lógica assíncrona.*/
	virtual void Activate() override;
	//~ End UBlueprintAsyncActionBase Interface
	
	// Expõe o delegate como Event Dispatcher no Blueprint, permitindo binding de eventos
	UPROPERTY(BlueprintAssignable)
	FOnConfirmScreenButtonClickedDelegate OnButtonClicked;
	
private:
	// Ponteiro fraco para o World, evita referências fortes e previne garbage collection indevido
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	// Tipo de modal armazenado temporariamente para uso na ativação assíncrona
	EConfirmScreenType CachedScreenType;
	// Título armazenado temporariamente até a tela ser criada
	FText CachedScreenTitle;
	// Mensagem armazenada temporariamente até a tela ser criada
	FText CachedScreenMessage;
};

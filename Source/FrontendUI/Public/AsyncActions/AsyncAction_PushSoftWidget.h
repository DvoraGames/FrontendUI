// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_PushSoftWidget.generated.h"

class UWidget_ActivatableBase;

// Define um delegate dinâmico (compatível com Blueprint) que aceita múltiplas conexões (multicast).
// Possui um único parâmetro: ponteiro para o widget que foi criado/pushed.
// PushedWidget é o nome do parâmetro que aparecerá no Blueprint.

/**
 * Define um delegate dinâmico multicast (compatível com Blueprint).
 * Dynamic: Permite serialização e binding em Blueprints via Event Dispatcher.
 * Multicast: Permite múltiplas funções conectadas que executam simultaneamente ao chamar Broadcast().
 * 
 * Disparado quando um widget é adicionado/pushed ao stack.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnPushedSoftWidgetDelegate,	// Nome do Delegate
	UWidget_ActivatableBase*,		// Tipo do Parametro
	PushedWidget					// Nome do parametro
	);

UCLASS()
class FRONTENDUI_API UAsyncAction_PushSoftWidget : public UBlueprintAsyncActionBase
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
		DisplayName="Push Soft Widget To Widget Stack")
	static UAsyncAction_PushSoftWidget* PushSoftWidget(
		// Objeto que fornece contexto do mundo, geralmente auto-preenchido no Blueprint.
		const UObject* WorldContextObject,
		// PlayerController que possui/controla o widget sendo criado.☻
		APlayerController* OwningPlayerController,
		// Soft reference para a classe do widget, permitindo carregamento assíncrono sem hard reference.
		TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass,
		// Tag de gameplay filtrada para mostrar apenas tags da categoria "Frontend.WidgetStack" no Blueprint.
		UPARAM(meta = (Categories= "Frontend.WidgetStack"))FGameplayTag InWidgetStackTag,
		// Define se o widget deve receber foco automaticamente após ser adicionado ao stack.
		bool bFocusOnPushedWidget = true
		);
	
	//~ Begin UBlueprintAsyncActionBase Interface
	/* Sobrescreve a função Activate() que é o "gatilho" das AsyncActions do Blueprint. 
	É chamado quando o nó é executado no BP, iniciando sua lógica assíncrona.*/
	virtual void Activate() override;
	//~ End UBlueprintAsyncActionBase Interface

	
	// Event Dispatcher visível no Blueprint para conectar eventos personalizados
	// Disparado quando o widget é criado, mas ainda não foi inserido no stack
	UPROPERTY(BlueprintAssignable)
	FOnPushedSoftWidgetDelegate OnWidgetCreatedBeforePush;
	
	// Event Dispatcher visível no Blueprint para conectar eventos personalizados
	// Disparado após o widget ser inserido no stack e estar pronto para uso
	UPROPERTY(BlueprintAssignable)
	FOnPushedSoftWidgetDelegate AfterPush;
	
private:
	// Ponteiro cache fraco (dangling pointers) para o World, previne referências fortes durante carregamento assíncrono
	// dangling pointers = ponteiros que apontam para uma memória que já foi liberada/destruída
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	// Ponteiro Cache fraco do PlayerController dono do widget
	TWeakObjectPtr<APlayerController> CachedOwningPC;
	// Ponteiro Cache da soft class do widget (carregamento assíncrono)
	TSoftClassPtr<UWidget_ActivatableBase> CachedSoftWidgetClass;
	// Cache da GameplayTag
	FGameplayTag CachedWidgetStackTag;
	// Cache do foco ao ser puxado
	bool bCachedFocusPushedWidget = false;
};

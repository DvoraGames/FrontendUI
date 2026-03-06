// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_PushSoftWidget.generated.h"

class UWidget_ActivatableBase;

/*
 * UAsyncActionPushSoftWidget
 * 
 * AsyncAction que carrega assincronamente um Widget via Soft Class Reference e o adiciona
 * ao Widget Stack especificado pela GameplayTag.
 *
 * Expõe dois pinos de execução no Blueprint:
 * - OnWidgetCreatedBeforePush: widget criado, ainda não inserido no stack — use para setup inicial.
 * - AfterPush: widget inserido no stack, visível e pronto para uso.
*/


// Delegate multicast compatível com Blueprint (Dynamic) — disparado quando um widget é adicionado/pushed ao stack.
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
		WorldContext = "WorldContextObject",  // Indica qual parâmetro fornece o contexto do UWorld.
		HidePin = "WorldContextObject",  // Oculta o pin do WorldContextObject no Blueprint, já que ele é inferido automaticamente pelo nó.
		BlueprintInternalUseOnly = "true"),  // Cria o nó assíncrono com pinos de execução de saída.
		DisplayName="Push Soft Widget To Widget Stack"  // Nome amigável que aparece no Blueprint em vez do nome real da função.
		)	
	static UAsyncAction_PushSoftWidget* PushSoftWidget(
		const UObject* WorldContextObject,  // Objeto que fornece contexto do mundo, geralmente auto-preenchido no Blueprint.
		APlayerController* OwningPlayerController,  // PlayerController dono do widget criado.
		TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass,  // Soft reference — permite carregamento assíncrono sem hard reference
		UPARAM(meta = 
			(Categories= "Frontend.WidgetStack"))FGameplayTag InWidgetStackTag,  // Tag filtrada para a categoria Frontend.WidgetStack
		bool bFocusOnPushedWidget = true  // Define se o widget foca automaticamente após o push.
		);
	
	//~ Begin UBlueprintAsyncActionBase Interface
	// Gatilho da AsyncAction — chamado quando o nó é executado no Blueprint, iniciando a lógica assíncrona
	virtual void Activate() override;
	//~ End UBlueprintAsyncActionBase Interface

	// ----------------------------------------------------------
	// Delegates
	// ----------------------------------------------------------
	
	// Disparado quando o widget foi criado, mas ainda não foi inserido no stack.
	UPROPERTY(BlueprintAssignable)
	FOnPushedSoftWidgetDelegate OnWidgetCreatedBeforePush;
	
	// Disparado após o widget ser inserido no stack e estar pronto para uso.
	UPROPERTY(BlueprintAssignable)
	FOnPushedSoftWidgetDelegate AfterPush;
	
private:
	// ----------------------------------------------------------
	// Cache
	// ----------------------------------------------------------
	
	//? Dangling pointers = ponteiros que apontam para uma memória que já foi liberada/destruída
	
	// Cache fraco do World — evita dangling pointer durante o carregamento assíncrono.
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	
    // Cache fraco do PlayerController dono do widget.
	TWeakObjectPtr<APlayerController> CachedOwningPC;
	
	// Soft class cacheada do widget a ser carregado. Soft Reference para não bloquear o carregamento do mapa.
	TSoftClassPtr<UWidget_ActivatableBase> CachedSoftWidgetClass;
	
    // Cache da GameplayTag que identifica o stack alvo onde o widget será inserido.
	FGameplayTag CachedWidgetStackTag;
	
    // Se true, o widget recebe foco automático após ser inserido no stack.
	bool bCachedFocusPushedWidget = false;
};

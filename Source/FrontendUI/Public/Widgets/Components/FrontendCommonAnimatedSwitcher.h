// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonAnimatedSwitcher.h"
#include "FrontendTypes/FrontendEnumTypes.h"

#include "FrontendCommonAnimatedSwitcher.generated.h"

/**
 * UFrontendCommonAnimatedSwitcher
 *
 * Extensão do CommonAnimatedSwitcher com suporte a modos de loop
 * e navegação sequencial (PreviousItem/NextItem) com controle de pending.
 */
UCLASS()
class FRONTENDUI_API UFrontendCommonAnimatedSwitcher : public UCommonAnimatedSwitcher
{
	GENERATED_BODY()
	
public:
	explicit UFrontendCommonAnimatedSwitcher(const FObjectInitializer& ObjectInitializer);
	
	// Retorna o modo de loop configurado
	UFUNCTION(BlueprintCallable, Category = "Transition")
	ECarouselLoopMode GetLoopMode() const { return LoopMode; };
	
	// Define o modo de loop em runtime
	UFUNCTION(BlueprintCallable, Category = "Transition")
	void SetLoopMode(ECarouselLoopMode NewLoopMode);
	
	// Move para o item anterior
	UFUNCTION(BlueprintCallable)
	void PreviousItem(bool& bDidMove);
	
	// Move para o próximo item
	UFUNCTION(BlueprintCallable)
	void NextItem(bool& bDidMove);
	
	// Cancela qualquer navegação pendente antes que seja processada
	void ClearPendingNavigation();
	
protected:
	// Chamado pelo Slate ao iniciar ou concluir uma transição animada
	virtual void HandleSlateIsTransitioningChanged(bool bIsTransitioning) override;
	
	// ----------------------------------------------------------
	// Properties
	// ----------------------------------------------------------
	
	// Modo de loop do Switcher
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transition")
	ECarouselLoopMode LoopMode;
	
	// True se há uma navegação para o próximo item aguardando processamento
	bool bPendingNext = false;
	
	// True se há uma navegação para o item anterior aguardando processamento
	bool bPendingPrev = false;
};

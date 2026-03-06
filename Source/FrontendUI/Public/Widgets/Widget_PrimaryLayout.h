// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "Widget_PrimaryLayout.generated.h"

class UCommonActivatableWidgetContainerBase;


/**
* UWidget_PrimaryLayout
*
* Widget raiz da UI do Frontend - ponto de entrada de todos os Widget Stacks.
* Armazena e gerencia um TMap de GameplayTag → WidgetStack, permitindo que o
* FrontendUISubsystem faça push de widgets nos stacks corretos por tag.
*
* Os stacks são registrados via Blueprint ao inicializar o layout.
*/
UCLASS(Abstract, BlueprintType, meta=(DisableNaiveTick))
class FRONTENDUI_API UWidget_PrimaryLayout : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	// Busca e retorna o Widget Stack correspondente à GameplayTag informada.
	UCommonActivatableWidgetContainerBase* FindWidgetStackByTag(const FGameplayTag& InTag) const;

protected:
	// Registra um Widget Stack no TMap vinculando-o à sua GameplayTag.
	UFUNCTION(BlueprintCallable)
	void RegisterWidgetStack(UPARAM(meta = 
		(Categories= "Frontend.WidgetStack"))FGameplayTag InStackTag, // Tag filtrada para Frontend.WidgetStack
		UCommonActivatableWidgetContainerBase* InStack);

private:
	//? Transient: não persiste entre sessões, reconstruído a cada carregamento do widget.

	// TMap que associa cada GameplayTag ao seu Widget Stack correspondente.
	UPROPERTY(Transient)
	TMap<FGameplayTag, UCommonActivatableWidgetContainerBase*> RegisterWidgetStackMap; 
};

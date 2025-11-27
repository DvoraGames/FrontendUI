// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "Widget_PrimaryLayout.generated.h"

class UCommonActivatableWidgetContainerBase;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNaiveTick))
class FRONTENDUI_API UWidget_PrimaryLayout : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	// Função Getter responsavel por procurar o Widget Stack usando a GamplayTag
	UCommonActivatableWidgetContainerBase* FindWidgetStackByTag(FGameplayTag& InTag) const;
	
protected:
	// Função responsavel por registrar os Widgets Stacks ao TMap (RegisterWidgetStackMap)
	UFUNCTION(BlueprintCallable)
	void RegisterWidgetStack(UPARAM(meta = (Categories= "Frontend.WidgetStack"))FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* InStack);
	// UPARAM, neste caso permite a escolha no seletor apenas de Tags relacionadas ao WidgetStack

private:
	// Propriedade para armazenar um Cache temporário dos widgets registrados.
	// Uma propriedade transitória é temporária e não persiste, seu valor é redefinido a cada carregamento.
	UPROPERTY(Transient) 
	TMap<FGameplayTag, UCommonActivatableWidgetContainerBase*> RegisterWidgetStackMap; 
};

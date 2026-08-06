// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_ToggleAction.generated.h"

class UListDataObject_Base;
class UListDataObject_Value;
enum class EToggleActionType : uint8;
class UToggleActionButton;
class UCommonActionWidget;

/**
 * UWidget_ToggleAction
 *
 * Widget responsável por exibir o botão de ação de toggle (ex: Mute/Link)
 * e o hint visual de input quando controlado via gamepad.
 */
UCLASS()
class FRONTENDUI_API UWidget_ToggleAction : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Retorna o botão de ação de toggle bindado.
	UToggleActionButton* GetToggleButton() const;
	
	// Retorna o widget de hint de input (ícone de ação) bindado.
	UCommonActionWidget* GetToggleActionWidget() const;
	
	// Atualiza a visibilidade e o estado visual do toggle com base no DataObject e no input atual.
	UFUNCTION(BlueprintCallable)
	void RefreshPresentation(const UListDataObject_Base* InDataObject, bool bIsEntrySelected, bool bIsGamepad) const;
	
private:
	// Botão que executa a ação de toggle.
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = true))
	UToggleActionButton* ToggleActionButton;
	
	// Widget que exibe o hint de input (ícone do botão do gamepad).
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = true))
	UCommonActionWidget* ToggleActionWidget;
};

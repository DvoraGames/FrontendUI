// DvoraGames All Rights Reserved

#include "Widgets/Components/Widget_ToggleAction.h"

#include "CommonActionWidget.h"
#include "Widgets/Components/ToggleActionButton.h"
#include "Widgets/Options/DataObjects/ListDataObject_Value.h"

UToggleActionButton* UWidget_ToggleAction::GetToggleButton() const
{
	// Retorna nulo se o botão não estiver bindado.
	if (!ToggleActionButton) return nullptr;
	
	return  ToggleActionButton;
}

UCommonActionWidget* UWidget_ToggleAction::GetToggleActionWidget() const
{
	// Retorna nulo se o widget de hint não estiver bindado.
	if (!ToggleActionWidget) return nullptr;
	
	return ToggleActionWidget;
}

void UWidget_ToggleAction::RefreshPresentation(const UListDataObject_Base* InDataObject, bool bIsEntrySelected, bool bIsGamepad) const
{
	// Aborta se o DataObject for inválido.
	if (!InDataObject) return;
	
	// Só processa se o DataObject for do tipo Value (suporta toggle action).
	if (const UListDataObject_Value* ValueDataObject = Cast<UListDataObject_Value>(InDataObject))
	{
		// Ajusta a visibilidade do botão conforme o tipo de ação configurado.
		GetToggleButton()->SetVisibilityByActionType(ValueDataObject);
		
		// Sincroniza o estado ativo/inativo do botão com o DataObject.
		GetToggleButton()->SetIsActionActive(ValueDataObject->GetbIsToggleActionActive());
		
		// O hint só deve aparecer quando o input for gamepad e a entry estiver selecionada.
		const bool bShouldShowToggleActionHint = bIsGamepad && bIsEntrySelected;
		
		// Mostra ou esconde o hint de input conforme a condição acima.
		GetToggleActionWidget()->SetVisibility(bShouldShowToggleActionHint ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

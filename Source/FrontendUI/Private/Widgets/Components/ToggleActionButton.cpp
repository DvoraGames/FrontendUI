// DvoraGames All Rights Reserved


#include "Widgets/Components/ToggleActionButton.h"

#include "CommonLazyImage.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "Widgets/Options/DataObjects/ListDataObject_Value.h"

void UToggleActionButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	// Garante que o botão já mostre o visual correto no editor/preconstruct.
	RefreshVisual();
}

void UToggleActionButton::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Garante que o botão mostre o visual correto ao ser construído em runtime.
	RefreshVisual();
}

void UToggleActionButton::SetToggleActionType(const EToggleActionType InActionType)
{
	// Aborta se o tipo já for o mesmo, evitando refresh desnecessário.
	if (ActionType == InActionType) return;
	
	// Atualiza o tipo de ação.
	ActionType = InActionType;
	
	// Atualiza o visual do botão.
	RefreshVisual();
}

void UToggleActionButton::SetIsActionActive(const bool bInActionActive)
{
	// Aborta se o tipo já for o mesmo, evitando refresh desnecessário.
	if (bIsActionActive == bInActionActive) return;
	
	// Atualiza o tipo de ação.
	bIsActionActive = bInActionActive;
	
	// Atualiza o visual do botão.
	RefreshVisual();
}

void UToggleActionButton::RefreshVisual()
{
	// Atualiza o ícone se a imagem estiver bindada.
	if (CommonLazyImage_ActionIcon)
	{
		// Resolve a textura conforme o tipo/estado atual.
		const TSoftObjectPtr<UTexture2D> ResolvedIcon = ResolveIconByActionType();
		
		// Aplica o ícone se ele estiver configurado.
		if (!ResolvedIcon.IsNull())
		{
			CommonLazyImage_ActionIcon->SetBrushFromLazyTexture(ResolvedIcon);
		}
	}
	
	// Notifica o Blueprint sobre o novo estado.
	BP_OnRefreshVisual(ActionType, bIsActionActive);
}

TSoftObjectPtr<UTexture2D> UToggleActionButton::ResolveIconByActionType() const
{
	// Escolhe o ícone conforme tipo e estado do toggle.
	switch (ActionType)
	{
	case EToggleActionType::Expand:
		return bIsActionActive ? Icon_Collapse : Icon_Expand;
	
	case EToggleActionType::Mute:
		return bIsActionActive ? Icon_Mute : Icon_Unmute;

	case EToggleActionType::Link:
		return bIsActionActive ? Icon_Link : Icon_Unlink;

	default:
		return nullptr;
	}
}

void UToggleActionButton::SetVisibilityByActionType(const UListDataObject_Base* InDataObject)
{
	// Aborta se o DataObject for inválido.
	if (!InDataObject) return;
	
	// Só processa DataObjects do tipo Value.
	if (const UListDataObject_Value* ValueDataObject = Cast<UListDataObject_Value>(InDataObject))
	{
		// Lê o tipo de ação configurado.
		const EToggleActionType DesiredActionType = ValueDataObject->GetToggleActionType(); 
	
		// Sincroniza tipo do botão.
		SetToggleActionType(DesiredActionType);
	
		// Sincroniza estado ativo do botão.
		SetIsActionActive(ValueDataObject->GetbIsToggleActionActive());
	
		// Esconde se não houver ação configurada.
		const ESlateVisibility ToggleButtonVisibility = (DesiredActionType != EToggleActionType::None)
		? ESlateVisibility::Visible 
		: ESlateVisibility::Hidden;
	
		// Aplica a visibilidade do botao
		SetVisibility(ToggleButtonVisibility);
	}
	
}


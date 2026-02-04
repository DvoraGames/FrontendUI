// DvoraGames All Rights Reserved


#include "Widgets/Options/Widget_OptionsScreen.h"

#include "ICommonInputModule.h"
#include "Input/CommonUIInputTypes.h"

#include "FrontendDegubHelper.h"

void UWidget_OptionsScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	// Verifica se o ResetAction é valido, ou seja, se foi setado e não é nulo
	if (ResetAction)
	{
			// Registra binding de UI action e retorna handle
			ResetActionHandle = RegisterUIActionBinding(
		 	FBindUIActionArgs(
				ResetAction,	// Ação (InputAction)
				true,			// Mostrar na Bound Action Bar
				FSimpleDelegate::CreateUObject(this, &ThisClass::OnResetBoundActionTriggered) // Callback quando acionado
			)
			);
	}
	
}

void UWidget_OptionsScreen::OnResetBoundActionTriggered()
{
	Debug::Print(TEXT("Reset Settings"));
}

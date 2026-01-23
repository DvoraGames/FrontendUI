// DvoraGames All Rights Reserved


#include "FrontendGameplayTags.h"

// Definindo as GameplayTags responsaveis por gerenciar os Menus
namespace FrontendGameplayTags
{
	/* Frontend Widgets Stacks */
	UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_Modal, "Frontend.WidgetStack.Modal");
	UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_GameMenu, "Frontend.WidgetStack.GameMenu");
	UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_GameHUD, "Frontend.WidgetStack.GameHUD");
	UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_Frontend, "Frontend.WidgetStack.Frontend");
	
	/* Frontend Widgets */
	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_PressAnyKeyScreen, "Frontend.Widget.PressAnyKeyScreen");
	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_MainMenuScreen, "Frontend.Widget.MainMenuScreen");
}
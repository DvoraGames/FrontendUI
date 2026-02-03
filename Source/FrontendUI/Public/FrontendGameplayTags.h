// DvoraGames All Rights Reserved

#pragma once

#include "NativeGameplayTags.h"

// Declarando as GameplayTags responsaveis por gerenciar os Menus
namespace FrontendGameplayTags
{
	/* Frontend Widgets Stacks */
	// Tag responsavel por gerenciar os Modais
	FRONTENDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_Modal);
	// Tag responsavel por gerenciar o Menus em jogo (Pausa, Inventario, etc)
	FRONTENDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_GameMenu);
	// Tag responsavel por gerenciar o HUD (Barra de vida, Stamina, etc)
	FRONTENDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_GameHUD);
	// Tag responsavel por gerenciar os Menus do frontend (Main, Settings, etc)
	FRONTENDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_Frontend);
	
	/* Frontend Widgets */
	// Tag responsavel pelo widget da tela Press Any Key
	FRONTENDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_PressAnyKeyScreen);
	// Tag responsavel pelo widget da tela do Menu Principal
	FRONTENDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_MainMenuScreen);
	// Tag responsavel pelo widget da tela Play Menu
	FRONTENDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_PlayScreen);
	// Tag responsavel pelo widget do Modal de confirmação
	FRONTENDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_ConfirmScreen);


}

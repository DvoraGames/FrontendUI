// DvoraGames All Rights Reserved

#pragma once

#include "NativeGameplayTags.h"

// Declaração externa das GameplayTags nativas do Frontend
namespace FrontendGameplayTags
{
	/*** Widgets Stacks ***/
	
	// Stack dos modais de confirmação
	FRONTENDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_Modal);
	
	// Stack dos menus em jogo (Pausa, Inventário, etc.)
	FRONTENDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_GameMenu);
	
	// Stack do HUD (Vida, Stamina, etc.)
	FRONTENDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_GameHUD);
	
	// Stack dos menus do Frontend (Main Menu, Settings, etc.)
	FRONTENDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_Frontend);
	
	
	/*** Widgets ***/

	// Tag do widget Press Any Key Screen
	FRONTENDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_PressAnyKeyScreen);
	// Tag do widget Main Menu Screen
	FRONTENDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_MainMenuScreen);
	// Tag do widget Options Screen
	FRONTENDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_OptionsScreen);
	
	// Tag do widget do Modal de confirmação
	FRONTENDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_ConfirmScreen);
	
	/*** Options Details View Images ***/

	// Tag de imagem para testes
	FRONTENDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Image_TesteImage);

}

// DvoraGames All Rights Reserved


#include "Widgets/Widget_ActivatableBase.h"
#include "Controllers/FrontendPlayerController.h"

AFrontendPlayerController* UWidget_ActivatableBase::GetOwningFrontendPC()
{
	// Verifica se ChachedOwningFrontendPC não é valido
	if (!ChachedOwningFrontendPC.IsValid())
	{
		// Se estiver vazio, tenta fazer um cast do Player Controller para a classe AFrontendPlayerController e armazena na variavel
		ChachedOwningFrontendPC = GetOwningPlayer<AFrontendPlayerController>();
	}
	
	// Se ChachedOwningFrontendPC for valido retorna ele, caso contrario, retorna nullptr
	return ChachedOwningFrontendPC.IsValid() ? ChachedOwningFrontendPC.Get() : nullptr;
}

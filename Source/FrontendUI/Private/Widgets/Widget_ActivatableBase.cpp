// DvoraGames All Rights Reserved


#include "Widgets/Widget_ActivatableBase.h"
#include "Controllers/FrontendPlayerController.h"

AFrontendPlayerController* UWidget_ActivatableBase::GetOwningFrontendPC()
{
	// Verifica se CachedOwningFrontendPC não é valido
	if (!CachedOwningFrontendPC.IsValid())
	{
		// Se estiver vazio, tenta fazer um cast do Player Controller para a classe AFrontendPlayerController e armazena na variavel
		CachedOwningFrontendPC = GetOwningPlayer<AFrontendPlayerController>();
	}
	
	// Se CachedOwningFrontendPC for valido retorna ele, caso contrario, retorna nullptr
	return CachedOwningFrontendPC.IsValid() ? CachedOwningFrontendPC.Get() : nullptr;
}

// DvoraGames All Rights Reserved


#include "Widgets/Widget_ActivatableBase.h"
#include "Controllers/FrontendPlayerController.h"

AFrontendPlayerController* UWidget_ActivatableBase::GetOwningFrontendPC()
{
	// Só faz o cast se o cache ainda não foi preenchido ou foi invalidado
	if (!CachedOwningFrontendPC.IsValid())
	{
		// Tenta converter o PlayerController dono para AFrontendPlayerController e armazena o resultado
		CachedOwningFrontendPC = GetOwningPlayer<AFrontendPlayerController>();
	}
	
	// Retorna o cache se for válido, nullptr caso o cast tenha falhado
	return CachedOwningFrontendPC.IsValid() ? CachedOwningFrontendPC.Get() : nullptr;
}

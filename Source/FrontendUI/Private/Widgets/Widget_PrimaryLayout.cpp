// DvoraGames All Rights Reserved


#include "Widgets/Widget_PrimaryLayout.h"

UCommonActivatableWidgetContainerBase* UWidget_PrimaryLayout::FindWidgetStackByTag(const FGameplayTag& InTag) const
{
	// Garante que a tag existe no mapa - crasha com log se não encontrar
	checkf(RegisterWidgetStackMap.Contains(InTag), TEXT("Can't find the widget stack by the Tag %s"), *InTag.ToString());
	
	// Retorna o stack vinculado à tag informada
	return RegisterWidgetStackMap.FindRef(InTag);
}

void UWidget_PrimaryLayout::RegisterWidgetStack(UPARAM(meta = (Categories= "Frontend.WidgetStack"))FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* InStack)
{
	// Só registra em runtime - ignora chamadas feitas no editor (Preview/Designer)
	if (!IsDesignTime())
	{
		// Previne registro duplicado - verifica se a Tag ainda não foi vinculada ao stack
		if (!RegisterWidgetStackMap.Contains(InStackTag))
		{
			// Vincula a tag ao stack e adiciona ao mapa
			RegisterWidgetStackMap.Add(InStackTag, InStack);
		}
	}
}

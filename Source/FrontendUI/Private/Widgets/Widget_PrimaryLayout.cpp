// DvoraGames All Rights Reserved


#include "Widgets/Widget_PrimaryLayout.h"

UCommonActivatableWidgetContainerBase* UWidget_PrimaryLayout::FindWidgetStackByTag(const FGameplayTag& InTag) const
{
	// Checa se o Map contem a Tag solicitada, caso contrario exibe uma mensagem de erro
	checkf(RegisterWidgetStackMap.Contains(InTag), TEXT("Can't find the widget stack by the Tag %s"), *InTag.ToString());
	
	// Retorna uma referecia do Widget Stack referente a Tag solicitada
	return RegisterWidgetStackMap.FindRef(InTag);
}

/* Função responsavel pela interação com a variavel RegisterWidgetStackMap, basicamente ela verifica se está no runtime
* e se o map já não contem o WidgetStack referente a Tag, prevenindo registrar o WidgetStack multiplas vezes*/
void UWidget_PrimaryLayout::RegisterWidgetStack(UPARAM(meta = (Categories= "Frontend.WidgetStack"))FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* InStack)
{
	// Verifica se está no runtime (tempo de execução)
	if (!IsDesignTime())
	{
		// Previne registro duplicado verificando se a Tag já existe no Map
		if (!RegisterWidgetStackMap.Contains(InStackTag))
		{
			// Adiciona a Tag requisitada (InStackTag) a pilha de widgets (InStack)
			RegisterWidgetStackMap.Add(InStackTag, InStack);
		}
	}
}

// DvoraGames All Rights Reserved


#include "Widgets/Components/FrontendCommonRotator.h"

#include "CommonTextBlock.h"

void UFrontendCommonRotator::SetSelectedOptionByText(const FText& InTextOption)
{
	// Busca e armazena o índice da primeira opção cujo texto seja igual ao procurado
	const int32 FoundIndex = TextLabels.IndexOfByPredicate(
		[InTextOption](const FText& TextItem)->bool
		{
			// Retorna true se o texto do item for igual ao procurado
			return TextItem.EqualTo(InTextOption);
		}
		);
	
	// Se encontrou:
	if (FoundIndex != INDEX_NONE)
	{
		// Atualiza o índice interno e dispara os eventos do rotator
		SetSelectedItem(FoundIndex);
	}
	else
	{
		// Fallback - valor não existe na lista, força o texto diretamente no componente visual
		MyText->SetText(InTextOption);
	}
}
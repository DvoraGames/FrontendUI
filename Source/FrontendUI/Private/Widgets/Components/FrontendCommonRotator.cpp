// DvoraGames All Rights Reserved


#include "Widgets/Components/FrontendCommonRotator.h"

#include "CommonTextBlock.h"

void UFrontendCommonRotator::SetSelectedOptionByText(const FText& InTextOption)
{
	// Retorna o índice do primeiro item cujo predicado (lambda) retornar true
	const int32 FoundIndex = TextLabels.IndexOfByPredicate(
		[InTextOption](const FText& TextItem)->bool
		{
			// Retorna true se o texto do item for igual ao procurado
			return TextItem.EqualTo(InTextOption);
		}
		);
	
	// Se encontrou a opção na lista:
	if (FoundIndex != INDEX_NONE)
	{
		// Define o índice selecionado (atualiza a lógica interna e dispara eventos)
		SetSelectedItem(FoundIndex);
	}
	else
	{
		/* Fallback: Se a opção não existe na lista (ex: valor customizado ou antigo),
		força o texto diretamente no componente visual para garantir que o usuário veja o valor. */
		MyText->SetText(InTextOption);
	}
}

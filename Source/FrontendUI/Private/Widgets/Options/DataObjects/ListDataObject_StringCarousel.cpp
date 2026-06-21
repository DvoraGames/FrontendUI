// DvoraGames All Rights Reserved

#include "Widgets/Options/DataObjects/ListDataObject_StringCarousel.h"

#include "Widgets/Options/OptionsDataInteractionHelper.h"

void UListDataObject_StringCarousel::OnCarouselInitiatedValueChange(const FText& InNewSelectedText)
{
    // Busca o índice do texto recebido no array de textos exibíveis
	const int32 FoundIndex = GetAvailableOptionsTextArray().IndexOfByPredicate(
		[InNewSelectedText](const FText& AvailableText)->bool
		{
			// Compara o texto disponível com o texto selecionado
			return AvailableText.EqualTo(InNewSelectedText);
		}
		);
	
    // Índice inválido ou dessincronizado entre arrays — ignora a mudança
	if (FoundIndex == INDEX_NONE || !GetAvailableOptionsStringArray().IsValidIndex(FoundIndex)) return;
	
    // Atualiza o texto e o valor interno com base no índice encontrado
	SetCurrentDisplayText(InNewSelectedText);
	SetCurrentStringValue(GetAvailableOptionsStringArray()[FoundIndex]);
		
	// Verifica se o Setter via Reflection está configurado
	if (DataDynamicSetter)
	{
		// Injeta o novo valor em formato de String no backend do jogo (GameUserSettings)
		DataDynamicSetter->SetValueFromString(GetCurrentStringValue());
	}
	
	// Notifica os widgets vinculados para reagirem à mudança e se redesenharem
	NotifyListDataModified(this);
}
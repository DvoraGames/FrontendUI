// DvoraGames All Rights Reserved

#include "Widgets/Options/DataObjects/ListDataObject_StringRotator.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"

// Avança para a próxima opção da lista. Se estiver na última, volta para a primeira (loop).
void UListDataObject_StringRotator::AdvanceToNextOption()
{
	// Aborta se não houver opções configuradas (String e Text) no carrossel
	if (GetAvailableOptionsStringArray().IsEmpty() && GetAvailableOptionsTextArray().IsEmpty()) return;
	
	// Descobre e armazena o índice atual na lista com base na string que está selecionada
	const int32 CurrentDisplayIndex = GetAvailableOptionsStringArray().IndexOfByKey(GetCurrentStringValue());
	
	// Incrementa o índice em +1 para o qual queremos avançar
	const int32 NextIndexToDisplay = CurrentDisplayIndex + 1;
	
	// Verifica se o índice incrementado existe dentro da lista
	if (GetAvailableOptionsStringArray().IsValidIndex(NextIndexToDisplay))
	{
		// Avança para o próximo valor se o índice for existir
		SetCurrentStringValue(GetAvailableOptionsStringArray()[NextIndexToDisplay]);
	}
	else
	{
		// volta para o primeiro indice (0) se não existir
		SetCurrentStringValue(GetAvailableOptionsStringArray()[0]);
	}
	
	// Sincroniza o texto de exibição com o novo valor selecionado
	TrySetDisplayTextFromStringValue(GetCurrentStringValue());
	
	// Verifica se o Setter via Reflection está configurado
	if (DataDynamicSetter)
	{
		// Injeta o novo valor em formato de String no backend do jogo (GameUserSettings)
		DataDynamicSetter->SetValueFromString(GetCurrentStringValue());
	}
	
	// Notifica os widgets vinculados para reagirem à mudança e se redesenharem
	NotifyListDataModified(this);
}

// Retorna para a opção anterior da lista. Se estiver na primeira, vai para a última (loop).
void UListDataObject_StringRotator::BackToPreviousOption()
{
	// Aborta se não houver opções configuradas (String e Text) no carrossel
	if (GetAvailableOptionsStringArray().IsEmpty() && GetAvailableOptionsTextArray().IsEmpty()) return;
	
	// Descobre e armazena o índice atual na lista com base na string que está selecionada
	const int32 CurrentDisplayIndex = GetAvailableOptionsStringArray().IndexOfByKey(GetCurrentStringValue());
	
	// Decrementa o índice em −1 para o qual queremos recuar
	const int32 PreviousIndexToDisplay = CurrentDisplayIndex - 1;
	
	// Verifica se o índice decrementado existe dentro da lista
	if (GetAvailableOptionsStringArray().IsValidIndex(PreviousIndexToDisplay))
	{
		// Recua para o valor anterior se o índice existir
		SetCurrentStringValue(GetAvailableOptionsStringArray()[PreviousIndexToDisplay]);
	}
	else
	{
		// Avança para o último Index se não existir
		SetCurrentStringValue(GetAvailableOptionsStringArray().Last());
	}
	
	// Sincroniza o texto de exibição com o novo valor selecionado
	TrySetDisplayTextFromStringValue(GetCurrentStringValue());
	
	// Verifica se o Setter via Reflection está configurado
	if (DataDynamicSetter)
	{
		// Injeta o novo valor em formato de String no backend do jogo (GameUserSettings)
		DataDynamicSetter->SetValueFromString(GetCurrentStringValue());
	}
	
	// Notifica os widgets vinculados para reagirem à mudança e se redesenharem
	NotifyListDataModified(this);
}

void UListDataObject_StringRotator::OnRotatorInitiatedValueChange(const FText& InNewSelectedText)
{
	// Busca o índice do texto recebido do Rotator no array de textos exibíveis
	const int32 FoundIndex = GetAvailableOptionsTextArray().IndexOfByPredicate(
		[InNewSelectedText](const FText& AvailableText)->bool
		{
			// Compara o texto disponível com o texto selecionado
			return AvailableText.EqualTo(InNewSelectedText);
		}
		);
	
	// Garante que o índice encontrado é válido em ambos os arrays antes de prosseguir
	if (FoundIndex != INDEX_NONE && GetAvailableOptionsStringArray().IsValidIndex(FoundIndex))
	{
		// Atualiza o texto e o valor interno com base no índice encontrado
		SetCurrentDisplayText(InNewSelectedText);
		SetCurrentStringValue(GetAvailableOptionsStringArray()[FoundIndex]);
		
		// Verifica se o Setter via Reflection está configurado
		if (DataDynamicSetter)
		{
			// Injeta o novo valor em formato de String no backend do jogo (GameUserSettings)
			DataDynamicSetter->SetValueFromString(GetCurrentStringValue());
		
			// Notifica os widgets vinculados para reagirem à mudança e se redesenharem
			NotifyListDataModified(this);
		}
	}
}

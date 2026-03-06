// DvoraGames All Rights Reserved

#include "Widgets/Options/DataObjects/ListDataObject_Carousel.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "FrontendDebugerHelper.h"

void UListDataObject_Carousel::OnDataObjectInitialized()
{
	// Por padrao seleciona a primeira opção se o array não estiver vazio
	if (!AvailableOptionsStringArray.IsEmpty())
	{
		CurrentStringValue = AvailableOptionsStringArray[0];
	}
	
	// Sobrescreve o fallback com o valor padrão configurado, se houver
	if (HasDefaultValue())
	{
		CurrentStringValue = GetDefaultValueAsString();
	}
	
	// Verifica se o Getter via Reflection está configurado
	if (DataDynamicGetter)
	{
		// Verifica se o retorno do Getter não está vazio antes de sobrescrever
		if (!DataDynamicGetter->GetValueAsString().IsEmpty())
		{
			// Sobrescreve o valor atual com a configuração real retornada pelo backend do jogo (GameUserSettings)
			CurrentStringValue = DataDynamicGetter->GetValueAsString();
		}
	}
	
	// Tenta atualizar o texto exibido. Se falhar (ex: array vazio ou dados dessincronizados), usa um fallback.
	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayText = FText::FromString(TEXT("Invalid Option"));
	}
	
}

void UListDataObject_Carousel::AddDynamicOption(const FString& InStringValue, const FText& InDisplayText)
{
	// Adiciona uma nova opção aos arrays de valores (FString) e de exibição (FText)
	AvailableOptionsStringArray.Add(InStringValue);
	AvailableOptionsTextArray.Add(InDisplayText);
}

// Avança para a próxima opção da lista. Se estiver na última, volta para a primeira (loop).
void UListDataObject_Carousel::AdvanceToNextOption()
{
	// Aborta se não houver opções configuradas (String e Text) no carrossel
	if (AvailableOptionsStringArray.IsEmpty() && AvailableOptionsTextArray.IsEmpty()) return;
	
	// Descobre e armazena o índice atual na lista com base na string que está selecionada
	const int32 CurrentDisplayIndex = AvailableOptionsStringArray.IndexOfByKey(CurrentStringValue);
	
	// Incrementa o índice em +1 para o qual queremos avançar
	const int32 NextIndexToDisplay = CurrentDisplayIndex + 1;
	
	// Verifica se o índice incrementado existe dentro da lista
	if (AvailableOptionsStringArray.IsValidIndex(NextIndexToDisplay))
	{
		// Avança para o próximo valor se o índice for existir
		CurrentStringValue = AvailableOptionsStringArray[NextIndexToDisplay];
	}
	else
	{
		// volta para o primeiro indice (0) se não existir
		CurrentStringValue = AvailableOptionsStringArray[0];
	}
	
	// Sincroniza o texto de exibição com o novo valor selecionado
	TrySetDisplayTextFromStringValue(CurrentStringValue);
	
	// Verifica se o Setter via Reflection está configurado
	if (DataDynamicSetter)
	{
		// Injeta o novo valor em formato de String no backend do jogo (GameUserSettings)
		DataDynamicSetter->SetValueFromString(CurrentStringValue);
		
		// Notifica os widgets vinculados para reagirem à mudança e se redesenharem
		NotifyListDataModified(this);
	}
}

// Retorna para a opção anterior da lista. Se estiver na primeira, vai para a última (loop).
void UListDataObject_Carousel::BackToPreviousOption()
{
	// Aborta se não houver opções configuradas (String e Text) no carrossel
	if (AvailableOptionsStringArray.IsEmpty() && AvailableOptionsTextArray.IsEmpty()) return;
	
	// Descobre e armazena o índice atual na lista com base na string que está selecionada
	const int32 CurrentDisplayIndex = AvailableOptionsStringArray.IndexOfByKey(CurrentStringValue);
	
	// Decrementa o índice em -1 para o qual queremos recuar
	const int32 PreviousIndexToDisplay = CurrentDisplayIndex - 1;
	
	// Verifica se o índice decrementado existe dentro da lista
	if (AvailableOptionsStringArray.IsValidIndex(PreviousIndexToDisplay))
	{
		// Recua para o valor anterior se o índice existir
		CurrentStringValue = AvailableOptionsStringArray[PreviousIndexToDisplay];
	}
	else
	{
		// Avança para o último Index se não existir
		CurrentStringValue = AvailableOptionsStringArray.Last();
	}
	
	// Sincroniza o texto de exibição com o novo valor selecionado
	TrySetDisplayTextFromStringValue(CurrentStringValue);
	
	// Verifica se o Setter via Reflection está configurado
	if (DataDynamicSetter)
	{
		// Injeta o novo valor em formato de String no backend do jogo (GameUserSettings)
		DataDynamicSetter->SetValueFromString(CurrentStringValue);
		
		// Notifica os widgets vinculados para reagirem à mudança e se redesenharem
		NotifyListDataModified(this);
	}
}

void UListDataObject_Carousel::OnRotatorInitiatedValueChange(const FText& InNewSelectedText)
{
	// Busca o índice do texto recebido do Rotator no array de textos exibíveis
	const int32 FoundIndex = AvailableOptionsTextArray.IndexOfByPredicate(
		[InNewSelectedText](const FText& AvailableText)->bool
		{
			// Compara o texto disponível com o texto selecionado
			return AvailableText.EqualTo(InNewSelectedText);
		}
		);
	
	// Garante que o índice encontrado é válido em ambos os arrays antes de prosseguir
	if (FoundIndex != INDEX_NONE && AvailableOptionsStringArray.IsValidIndex(FoundIndex))
	{
		// Atualiza o texto e o valor interno com base no índice encontrado
		CurrentDisplayText = InNewSelectedText;
		CurrentStringValue = AvailableOptionsStringArray[FoundIndex];
		
		// Verifica se o Setter via Reflection está configurado
		if (DataDynamicSetter)
		{
			// Injeta o novo valor em formato de String no backend do jogo (GameUserSettings)
			DataDynamicSetter->SetValueFromString(CurrentStringValue);
		
			// Notifica os widgets vinculados para reagirem à mudança e se redesenharem
			NotifyListDataModified(this);
		}
	}
}

bool UListDataObject_Carousel::CanResetBackToDefaultValue() const
{
	// Só permite reset se há valor padrão definido e o valor atual é diferente dele
	return HasDefaultValue() && CurrentStringValue != GetDefaultValueAsString();
}

bool UListDataObject_Carousel::TryResetBackToDefaultValue()
{
	// Se tiver valor padrão e o valor atual for diferente dele
	if (CanResetBackToDefaultValue())
	{
		// Reverte o valor interno para o padrão configurado
		CurrentStringValue = GetDefaultValueAsString();
		
		// Sincroniza o texto de exibição com o valor revertido
		TrySetDisplayTextFromStringValue(CurrentStringValue);
		
		// Verifica se o Setter via Reflection está configurado
		if (DataDynamicSetter)
		{
			// Injeta o novo valor em formato de String no backend do jogo (GameUserSettings)
			DataDynamicSetter->SetValueFromString(CurrentStringValue);
		
			// Notifica os widgets vinculados para reagirem à mudança e se redesenharem
			NotifyListDataModified(this);
			
			// Retorna que o Reset foi bem-sucedido
			return true;
		}
	}
	
	// Retorna que o Reset não foi feito
	return false;
}

bool UListDataObject_Carousel::TrySetDisplayTextFromStringValue(const FString& InStringValue)
{
	// Busca o índice do valor interno no array de strings
	const int32 CurrentFoundIndex = AvailableOptionsStringArray.IndexOfByKey(InStringValue);
	
	// Verifica se o índice encontrado também é válido no array de textos exibíveis
	if (AvailableOptionsTextArray.IsValidIndex(CurrentFoundIndex))
	{
		// Atualiza o texto de exibição com o texto correspondente ao índice
		CurrentDisplayText = AvailableOptionsTextArray[CurrentFoundIndex];
		return true;
	}
	
    // Falha - índice inválido, arrays dessincronizados ou valor não encontrado
	return false;
}

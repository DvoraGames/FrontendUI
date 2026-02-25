// DvoraGames All Rights Reserved

#include "Widgets/Options/DataObjects/ListDataObject_Carousel.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "FrontendDebugerHelper.h"



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
	if (AvailableOptionsStringArray.IsEmpty() && AvailableOptionsTextArray.IsEmpty())
	{
		return;
	}
	
	// Descobre o índice atual na lista com base na string que está selecionada
	const int32 CurrentDisplayIndex = AvailableOptionsStringArray.IndexOfByKey(CurrentStringValue);
	// Calcula o índice para o qual queremos avançar (+1)
	const int32 NextIndexToDisplay = CurrentDisplayIndex + 1;
	
	// Verifica se esse novo índice existe dentro da lista
	const bool bIsNextIndexValid = AvailableOptionsStringArray.IsValidIndex(NextIndexToDisplay);
	
	if (bIsNextIndexValid)
	{
		// Avança para o próximo valor se o índice for válido
		CurrentStringValue = AvailableOptionsStringArray[NextIndexToDisplay];
	}
	else
	{
		// Se o valor não for valido (passar do limite) volta para o Index 0
		CurrentStringValue = AvailableOptionsStringArray[0];
	}
	
	// Sincroniza o texto de exibição com o novo valor selecionado
	TrySetDisplayTextFromStringValue(CurrentStringValue);
	
	// Se o Setter configurado via Reflection. for valido
	if (DataDynamicSetter)
	{
		// Injeta o novo valor (convertido em string) diretamente na variável original do jogo
		DataDynamicSetter->SetValueFromString(CurrentStringValue);
		
		Debug::Print(TEXT("DataDynamicSetter is used. The lasted value from Getter: ") + DataDynamicGetter->GetValueAsString());
		
		// Dispara o evento de modificação deste DataObject, avisando os widgets (como o ListView) para se redesenharem
		NotifyListDataModified(this);
	}
}

// Retorna para a opção anterior da lista. Se estiver na primeira, vai para a última (loop).
void UListDataObject_Carousel::BackToPreviousOption()
{
	// Aborta se não houver opções configuradas (String e Text) no carrossel
	if (AvailableOptionsStringArray.IsEmpty() && AvailableOptionsTextArray.IsEmpty())
	{
		return;
	}
	
	// Descobre o índice atual na lista com base na string que está selecionada
	const int32 CurrentDisplayIndex = AvailableOptionsStringArray.IndexOfByKey(CurrentStringValue);
	// Calcula o índice para o qual queremos recuar (-1)
	const int32 PreviousIndexToDisplay = CurrentDisplayIndex - 1;
	
	// Verifica se esse novo índice existe dentro da lista
	const bool bIsNextIndexValid = AvailableOptionsStringArray.IsValidIndex(PreviousIndexToDisplay);
	
	if (bIsNextIndexValid)
	{
		// Retorna para o índice anterior se ele existir na lista (maior ou igual a 0)
		CurrentStringValue = AvailableOptionsStringArray[PreviousIndexToDisplay];
	}
	else
	{
		// Se o valor não for valido (passar do limite) voi para o último index
		CurrentStringValue = AvailableOptionsStringArray.Last();
	}
	
	// Sincroniza o texto de exibição com o novo valor selecionado
	TrySetDisplayTextFromStringValue(CurrentStringValue);
	
	// Se o Setter configurado via Reflection. for valido
	if (DataDynamicSetter)
	{
		// Injeta o novo valor (convertido em string) diretamente na variável original do jogo
		DataDynamicSetter->SetValueFromString(CurrentStringValue);
		
		Debug::Print(TEXT("DataDynamicSetter is used. The lasted value from Getter: ") + DataDynamicGetter->GetValueAsString());
		
		// Dispara o evento de modificação deste DataObject, avisando os widgets (como o ListView) para se redesenharem
		NotifyListDataModified(this);
	}
}

void UListDataObject_Carousel::OnDataObjectInitialized()
{
	// Por padrão, inicializa o carrossel na primeira opção da lista senão estiver vazio
	if (!AvailableOptionsStringArray.IsEmpty())
	{
		CurrentStringValue = AvailableOptionsStringArray[0];
	}
	
	// Se o Getter configurado via Reflection for valido
	if (DataDynamicGetter)
	{
		// Verifica se o retorno do Getter não está vazio
		if (!DataDynamicGetter->GetValueAsString().IsEmpty())
		{
			// Sobrescreve o valor padrão/fallback com a configuração real retornada pelo backend do jogo
			CurrentStringValue = DataDynamicGetter->GetValueAsString();
		}
	}
	
	// Tenta atualizar o texto exibido. Se falhar (ex: array vazio ou dados dessincronizados), usa um fallback.
	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayText = FText::FromString(TEXT("Invalid Option"));
	}
	
}

bool UListDataObject_Carousel::TrySetDisplayTextFromStringValue(const FString& InStringValue)
{
	// Procura o índice do valor interno
	const int32 CurrentFoundIndex = AvailableOptionsStringArray.IndexOfByKey(InStringValue);
	
    // Verifica se esse mesmo índice existe no array de textos visuais
	if (AvailableOptionsTextArray.IsValidIndex(CurrentFoundIndex))
	{
		// Atualiza o texto selecionado e reporta sucesso
		CurrentDisplayText = AvailableOptionsTextArray[CurrentFoundIndex];
		
		return true;
	}
	
	// Falha: índice inválido (provavelmente arrays com tamanhos diferentes ou valor não encontrado)
	return false;
}

// DvoraGames All Rights Reserved

#include "Widgets/Options/DataObjects/ListDataObject_StringRotator.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"

void UListDataObject_StringRotator::OnRotatorInitiatedValueChange(const FText& InNewSelectedText)
{
	// Cacheia os arrays para evitar múltiplas chamadas aos getters.
	const TArray<FText> AvailableTexts = GetAvailableOptionsTextArray();
	const TArray<FString> AvailableStrings = GetAvailableOptionsStringArray();

	// Procura o índice do texto selecionado dentro da lista de textos exibidos.
	const int32 FoundIndex = AvailableTexts.IndexOfByPredicate(
		[InNewSelectedText](const FText& AvailableText)-> bool
		{
			// Compara o texto disponível com o texto selecionado
			return AvailableText.EqualTo(InNewSelectedText);
		}
	);

	// Aborta se o índice inválido ou não existir no array de valores internos.
	if (FoundIndex == INDEX_NONE || !AvailableStrings.IsValidIndex(FoundIndex)) return;

	// Atualiza o texto atualmente exibido.
	SetCurrentDisplayText(InNewSelectedText);
	
	// Atualiza o valor interno usando o array já cacheado.
	SetCurrentStringValue(AvailableStrings[FoundIndex]);

	// Recalcula o estado de expansão dos filhos.
	RefreshChildrenExpansionState();

	// Verifica se existe um setter dinâmico configurado.
	if (DataDynamicSetter)
	{
		// Salva o valor atual no backend do jogo.
		DataDynamicSetter->SetValueFromString(GetCurrentStringValue());
	}

	// Notifica os widgets vinculados sobre a alteração.
	NotifyListDataModified(this);
}

void UListDataObject_StringRotator::SetNavigationMode(const ERotatorLoopMode InNavigationMode)
{
	// Define o modo de navegação do rotator.
	NavigationMode = InNavigationMode;
}

void UListDataObject_StringRotator::AdvanceToNextOption()
{
	// Aborta se não houver opções configuradas (String e Text) no carrossel
	if (GetAvailableOptionsStringArray().IsEmpty() || GetAvailableOptionsTextArray().IsEmpty()) return;

	// Obtém o índice atual na lista com base na string que está selecionada
	const int32 CurrentDisplayIndex = GetAvailableOptionsStringArray().IndexOfByKey(GetCurrentStringValue());

	// Incrementa o índice em +1 para o qual queremos avançar
	int32 NextIndexToDisplay = CurrentDisplayIndex + 1;
	
	// Verifica se o proximo índice está fora limites.
	if (!GetAvailableOptionsStringArray().IsValidIndex(NextIndexToDisplay))
	{
		// Decide o comportamento no limite conforme o modo de navegação.
		switch (NavigationMode)
		{
			// Volta para a primeira opção.
			case ERotatorLoopMode::Infinite:
				NextIndexToDisplay = 0;
				break;
				
			// Mantém a opção atual.
			case ERotatorLoopMode::Stop:
				NextIndexToDisplay = CurrentDisplayIndex;
				break;
				
			default:
				break;
				
		}
	}
		
	// Atualiza o valor interno para a próxima opção.
	SetCurrentStringValue(GetAvailableOptionsStringArray()[NextIndexToDisplay]);

	// Sincroniza o texto de exibição com o novo valor selecionado
	TrySetDisplayTextFromStringValue(GetCurrentStringValue());

	// Verifica se existe um setter dinâmico configurado.
	if (DataDynamicSetter)
	{
		// Injeta o novo valor em formato de String no backend do jogo (GameUserSettings)
		DataDynamicSetter->SetValueFromString(GetCurrentStringValue());
	}

	// Notifica os widgets vinculados para reagirem à mudança e se redesenharem
	NotifyListDataModified(this);
}

void UListDataObject_StringRotator::BackToPreviousOption()
{
	// Aborta se não houver opções configuradas (String e Text) no carrossel
	if (GetAvailableOptionsStringArray().IsEmpty() || GetAvailableOptionsTextArray().IsEmpty()) return;

	// Obtém o índice atual na lista com base na string que está selecionada
	const int32 CurrentDisplayIndex = GetAvailableOptionsStringArray().IndexOfByKey(GetCurrentStringValue());

	// Decrementa o índice em −1 para o qual queremos recuar
	int32 PreviousIndexToDisplay = CurrentDisplayIndex - 1;
	
	// Verifica se o proximo índice está fora limites.
	if (!GetAvailableOptionsStringArray().IsValidIndex(PreviousIndexToDisplay))
	{
		// Decide o comportamento no limite conforme o modo de navegação.
		switch (NavigationMode)
		{
			// Vai para a última opção.
			case ERotatorLoopMode::Infinite:
				PreviousIndexToDisplay = GetAvailableOptionsStringArray().Num()-1;
				break;
			
			// Mantém a opção atual.
			case ERotatorLoopMode::Stop:
				PreviousIndexToDisplay = CurrentDisplayIndex;
				break;
				
			default:
				break;
				
		}
	}
		
	// Atualiza o valor interno para a opção anterior.
	SetCurrentStringValue(GetAvailableOptionsStringArray()[PreviousIndexToDisplay]);

	// Sincroniza o texto de exibição com o novo valor selecionado
	TrySetDisplayTextFromStringValue(GetCurrentStringValue());

	// Verifica se existe um setter dinâmico configurado.
	if (DataDynamicSetter)
	{
		// Injeta o novo valor em formato de String no backend do jogo (GameUserSettings)
		DataDynamicSetter->SetValueFromString(GetCurrentStringValue());
	}

	// Notifica os widgets vinculados para reagirem à mudança e se redesenharem
	NotifyListDataModified(this);
}

//********** UListData_Bool **********//
void UListDataObject_BoolRotator::OnDataObjectInitialized()
{
	// Inicializa as opções booleanas internas.
	TryInitBoolValues();

	// Registra o valor que deve desativar ou colapsar os filhos.
	AddChildrenDeactivateStringValues({FalseString});

	// Executa a inicialização da classe base.
	Super::OnDataObjectInitialized();
}

void UListDataObject_BoolRotator::OverrideBoolDisplayTexts(
	const FText& InNewTrueDisplayText,
	const FText& InNewFalseDisplayText)
{
	// Atualiza o texto exibido para o estado true.
	TrueDisplayText = InNewTrueDisplayText;
	
	// Atualiza o texto exibido para o estado false.
	FalseDisplayText = InNewFalseDisplayText;
}

void UListDataObject_BoolRotator::SetBoolDefaultValue(const bool Value)
{
	// Define o valor padrão em formato de string.
	const FString DefaultBoolString = Value ? TrueString : FalseString;

	// Aplica o valor padrão da opção.
	SetDefaultValueFromString(DefaultBoolString);
}

void UListDataObject_BoolRotator::TryInitBoolValues()
{
	// Registra a opção booleana false.
	AddDynamicOption(FalseString, FalseDisplayText);
	
	// Registra a opção booleana true.
	AddDynamicOption(TrueString, TrueDisplayText);
}

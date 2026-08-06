// DvoraGames All Rights Reserved

#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"

void UListDataObject_String::OnDataObjectInitialized()
{
	// Executa a inicialização da classe base.
	Super::OnDataObjectInitialized();
	
	// Define que a expansão dos filhos será controlada automaticamente.
	SetChildrenMode(EEntryChildrenMode::ExpandableAuto);
	
	// Define a primeira opção como valor inicial, se existir.
	if (!AvailableOptionsStringArray.IsEmpty())
	{
		CurrentStringValue = AvailableOptionsStringArray[0];
	}

	// Sobrescreve com o valor padrão configurado, se existir.
	if (HasDefaultValue())
	{
		CurrentStringValue = GetDefaultValueAsString();
	}

	// Sobrescreve com o valor atual retornado pelo backend, se válido.
	if (DataDynamicGetter && !DataDynamicGetter->GetValueAsString().IsEmpty())
	{
		CurrentStringValue = DataDynamicGetter->GetValueAsString();
	}
	
	// Atualiza o texto de exibição com base no valor atual.
	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayText = FText::FromString(TEXT("Invalid Option"));
	}
	
	// Define o estado inicial de expansão dos filhos.
	SetbIsExpanded(ShouldActivateChildren());
}

void UListDataObject_String::AddDynamicOption(const FString& InStringValue, const FText& InDisplayText)
{
	// Registra nova opção nos arrays de valor interno (FString) e de texto de exibição (FText).
	AvailableOptionsStringArray.Add(InStringValue);
	AvailableOptionsTextArray.Add(InDisplayText);
}

bool UListDataObject_String::CanResetBackToDefaultValue() const
{
	// Só permite reset se há valor padrão definido e o valor atual é diferente dele
	return HasDefaultValue() && CurrentStringValue != GetDefaultValueAsString();
}

bool UListDataObject_String::TryResetBackToDefaultValue()
{
	// Retorna false se não houver valor padrão ou se o valor atual já for o padrão.
	if (!CanResetBackToDefaultValue()) return false;
	
	// Restaura o valor interno para o valor padrão.
	CurrentStringValue = GetDefaultValueAsString();
	
	// Atualiza o texto exibido com base no valor restaurado.
	TrySetDisplayTextFromStringValue(CurrentStringValue);
	
	// Atualiza o estado de expansão dos filhos.
	RefreshChildrenExpansionState();
	
	// Verifica se o Setter via Reflection está configurado
	if (DataDynamicSetter)
	{
		// Salva o valor restaurado no backend do jogo.
		DataDynamicSetter->SetValueFromString(CurrentStringValue);
	}
	
	// Notifica os widgets vinculados sobre a alteração.
	NotifyListDataModified(this);
		
	// Retorna true se o reset foi concluído com sucesso.
	return true;
}

bool UListDataObject_String::TrySetDisplayTextFromStringValue(const FString& InStringValue)
{
	// Busca o índice do valor atual no array de opções internas.
	const int32 CurrentFoundIndex = AvailableOptionsStringArray.IndexOfByKey(InStringValue);
	
	// Verifica se o índice encontrado é válido no array de textos exibidos.
	if (AvailableOptionsTextArray.IsValidIndex(CurrentFoundIndex))
	{
		// Atualiza o texto exibido com base no índice encontrado.
		CurrentDisplayText = AvailableOptionsTextArray[CurrentFoundIndex];
		return true;
	}
	
	// Retorna false se o valor não for encontrado nas opções cadastradas.
	return false;
}

void UListDataObject_String::AddChildrenDeactivateStringValues(const TArray<FString>& InStringArray)
{
	// Aborta se o array recebido estiver vazio.
	if (InStringArray.IsEmpty()) return;
	
	// Registra os valores que devem desativar ou colapsar os filhos.
	for (FString InString : InStringArray)
	{
		CollapseOrDisableStringArray.Add(InString);
	}
}

bool UListDataObject_String::ShouldActivateChildren() const
{
	// Retorna false se o valor atual estiver na lista de valores desativados.
	return !GetCollapseOrDisableStringArray().ContainsByPredicate(
		[&](const FString& InDeactivatedString)->bool
		{
			return CurrentStringValue.Equals(InDeactivatedString, ESearchCase::IgnoreCase);
		}
	);
}

void UListDataObject_String::RefreshChildrenExpansionState()
{
	// Aborta se esta entry não usar expansão automática.
	if (!UsesAutoExpansion()) return;
	
	// Atualiza o estado de expansão com base no valor atual.
	SetbIsExpanded(ShouldActivateChildren());
}

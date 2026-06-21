// DvoraGames All Rights Reserved


#include "Widgets/Options/DataObjects/ListDataObject_String.h"

#include "Widgets/Options/OptionsDataInteractionHelper.h"

void UListDataObject_String::OnDataObjectInitialized()
{
	Super::OnDataObjectInitialized();
	
	// Seleciona a primeira opção como valor inicial padrão
	if (!AvailableOptionsStringArray.IsEmpty())
	{
		CurrentStringValue = AvailableOptionsStringArray[0];
	}

	// Sobrescreve com o valor padrão configurado no DataAsset, se houver
	if (HasDefaultValue())
	{
		CurrentStringValue = GetDefaultValueAsString();
	}

	// Verifica se o Getter via Reflection está configurado e se o retorno do Getter não está vazio
	if (DataDynamicGetter && !DataDynamicGetter->GetValueAsString().IsEmpty())
	{
		// Sobrescreve o valor atual com a configuração real retornada pelo backend do jogo (GameUserSettings)
		CurrentStringValue = DataDynamicGetter->GetValueAsString();
	}

	// Sincroniza o texto de exibição - usa fallback se o valor não for encontrado nas opções
	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayText = FText::FromString(TEXT("Invalid Option"));
	}
	
}

void UListDataObject_String::AddDynamicOption(const FString& InStringValue, const FText& InDisplayText)
{
	// Registra nova opção nos arrays de valor interno (FString) e de texto de exibição (FText)
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
	// Aborta se não há valor padrão ou o valor atual já é o padrão
	if (!CanResetBackToDefaultValue()) return false;
	
	// Reverte o valor interno para o padrão configurado
	CurrentStringValue = GetDefaultValueAsString();
	
	// Sincroniza o texto de exibição com o valor revertido
	TrySetDisplayTextFromStringValue(CurrentStringValue);
	
	// Verifica se o Setter via Reflection está configurado
	if (DataDynamicSetter)
	{
		// Injeta o novo valor em formato de String no backend do jogo (GameUserSettings)
		DataDynamicSetter->SetValueFromString(CurrentStringValue);
	}
	
	// Notifica os widgets vinculados para reagirem à mudança e se redesenharem
	NotifyListDataModified(this);
		
	// Retorna que o Reset foi bem-sucedido
	return true;
}

bool UListDataObject_String::TrySetDisplayTextFromStringValue(const FString& InStringValue)
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
	
	// Índice inválido — arrays dessincronizados ou valor não cadastrado
	return false;
}
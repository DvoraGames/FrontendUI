// DvoraGames All Rights Reserved

#include "Widgets/Options/DataObjects/ListDataObject_StringCarousel.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"

void UListDataObject_StringCarousel::OnCarouselInitiatedValueChange(const FText& InNewSelectedText)
{
	// Armazena os arrays de opções para evitar chamadas repetidas aos getters.
	const TArray<FText> AvailableTexts = GetAvailableOptionsTextArray();
	const TArray<FString> AvailableStrings = GetAvailableOptionsStringArray();
	
    // Busca o índice do texto recebido no array de textos exibidos.
	const int32 FoundIndex = AvailableTexts.IndexOfByPredicate(
		[InNewSelectedText](const FText& AvailableText)->bool
		{
			return AvailableText.EqualTo(InNewSelectedText);
		}
		);
	
    // Retorna se o índice encontrado for inválido ou não existir no array de valores internos.
	if (FoundIndex == INDEX_NONE || !AvailableStrings.IsValidIndex(FoundIndex)) return;
	
    // Atualiza o texto e o valor interno com base no índice encontrado.
	SetCurrentDisplayText(InNewSelectedText);
	SetCurrentStringValue(GetAvailableOptionsStringArray()[FoundIndex]);
		
	// Verifica se o Setter via Reflection está configurado
	if (DataDynamicSetter)
	{
		// Salva o valor atual no backend do jogo.
		DataDynamicSetter->SetValueFromString(GetCurrentStringValue());
	}
	
	// Notifica os widgets vinculados sobre a alteração.
	NotifyListDataModified(this);
}

//********** UListData_BoolCarousel **********//

void UListDataObject_BoolCarousel::OnDataObjectInitialized()
{
	// Inicializa as opções booleanas internas.
	TryInitBoolValues();
	
	// Registra o valor que deve desativar ou colapsar os filhos.
	AddChildrenDeactivateStringValues({ FalseString });
	
	// Executa a inicialização da classe base.
	Super::OnDataObjectInitialized();
}

void UListDataObject_BoolCarousel::OverrideBoolDisplayTexts(
	const FText& InNewTrueDisplayText,
	const FText& InNewFalseDisplayText)
{
	// Atualiza o texto exibido para o estado false.
	FalseDisplayText = InNewFalseDisplayText;
	
	// Atualiza o texto exibido para o estado true.
	TrueDisplayText = InNewTrueDisplayText;
}

void UListDataObject_BoolCarousel::SetBoolDefaultValue(const bool Value)
{
	// Define o valor padrão em formato de string.
	const FString DefaultBoolString = Value ? TrueString : FalseString;
	
	// Aplica o valor padrão da opção.
	SetDefaultValueFromString(DefaultBoolString);
}

void UListDataObject_BoolCarousel::TryInitBoolValues()
{
	// Registra a opção booleana false.
	AddDynamicOption(FalseString, FalseDisplayText);
	
	// Registra a opção booleana true.
	AddDynamicOption(TrueString, TrueDisplayText);
}
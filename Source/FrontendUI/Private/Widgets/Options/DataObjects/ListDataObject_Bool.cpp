// DvoraGames All Rights Reserved


#include "Widgets/Options/DataObjects/ListDataObject_Bool.h"

#include "Widgets/Options/OptionsDataInteractionHelper.h"

void UListDataObject_Bool::OnDataObjectInitialized()
{
	Super::OnDataObjectInitialized();
	
	// Popula os arrays com os textos e valores internos
	AddOption(TEXT("false"), FalseDisplayText);
	AddOption(TEXT("true"), TrueDisplayText);
	
	// Fallback — Seleciona primeira opção (false)
	CurrentStringValue = BoolOptionsStringArray[0];
	
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
	
	// Deriva o estado bool a partir do valor final sincronizado
	CurrentBoolState = CurrentStringValue == BoolOptionsStringArray[1];
	
	//  Se tiver filho, define o estado de expansão inicial conforme o estado booleano.
	if (HasAnyChildListData())
	{
		SetIsExpanded(CurrentBoolState);
	}
}

void UListDataObject_Bool::SetDefaultBoolValue(const bool InDefaultValue)
{
	// Converte o bool para string e delega para o setter de DefaultValue da classe pai
	SetDefaultValueFromString(InDefaultValue ? TEXT("true") : TEXT("false"));
}

void UListDataObject_Bool::AddOption(const FString& InStringValue, const FText& InDisplayText)
{
	// Registra o valor interno e o texto de exibição nos arrays correspondentes
	BoolOptionsStringArray.Add(InStringValue);
	BoolOptionsTextArray.Add(InDisplayText);
}

void UListDataObject_Bool::SetBoolTexts(const FText& InFalseStateText, const FText& InTrueStateText)
{
	// Sobrescrever os textos padrão
	FalseDisplayText = InFalseStateText;
	TrueDisplayText = InTrueStateText;
}

void UListDataObject_Bool::OnCarouselInitiatedValueChange(const FText& InNewSelectedText)
{
	// Busca o índice do texto recebido no array de textos exibíveis
	const int32 FoundIndex = BoolOptionsTextArray.IndexOfByPredicate(
		[InNewSelectedText](const FText& AvailableText)->bool
		{
			// Compara o texto disponível com o texto selecionado
			return AvailableText.EqualTo(InNewSelectedText);
		}
		);
	
	// Índice inválido ou dessincronizado entre arrays — ignora a mudança
	if (FoundIndex == INDEX_NONE || !BoolOptionsStringArray.IsValidIndex(FoundIndex)) return;
	
	// Atualiza o texto e o valor interno com base no índice encontrado
	CurrentDisplayText = InNewSelectedText;
	CurrentStringValue = BoolOptionsStringArray[FoundIndex];
	
	// Deriva o estado bool a partir do novo valor selecionado
	CurrentBoolState = (CurrentStringValue == BoolOptionsStringArray[1]);
		
	// Verifica se o Setter via Reflection está configurado
	if (DataDynamicSetter)
	{
		// Injeta o novo valor em formato de String no backend do jogo (GameUserSettings)
		DataDynamicSetter->SetValueFromString(CurrentStringValue);
	}
	
	// Notifica os widgets vinculados para reagirem à mudança e se redesenharem
	NotifyListDataModified(this);
}

bool UListDataObject_Bool::CanResetBackToDefaultValue() const
{
	// Só permite reset se há valor padrão definido e o valor atual é diferente dele
	return HasDefaultValue() && CurrentStringValue != GetDefaultValueAsString();
}

bool UListDataObject_Bool::TryResetBackToDefaultValue()
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

bool UListDataObject_Bool::TrySetDisplayTextFromStringValue(const FString& InStringValue)
{
	// Busca o índice do valor interno no array de strings
	const int32 CurrentFoundIndex = BoolOptionsStringArray.IndexOfByKey(InStringValue);
	
	// Verifica se o índice encontrado também é válido no array de textos exibíveis
	if (BoolOptionsStringArray.IsValidIndex(CurrentFoundIndex))
	{
		// Atualiza o texto de exibição com o texto correspondente ao índice
		CurrentDisplayText = BoolOptionsTextArray[CurrentFoundIndex];
		return true;
	}
	
	// Índice inválido — arrays dessincronizados ou valor não cadastrado
	return false;
}

// ----------------------------------------------------------
// Children Manager — IIListDataWithChildren
// ----------------------------------------------------------

void UListDataObject_Bool::AddChildListData(UListDataObject_Base* InChildListData)
{
	// Aborta se o filho for inválido.
	if (!InChildListData) return;
	
	// Define esta coleção como pai do filho.
	InChildListData->SetParentData(this);
	
	// Armazena a opção filha a coleção interna desta aba.
	ChildListDataArray.Add(InChildListData);
	
	// Inicializa o DataObject filho.
	InChildListData->InitDataObject();
}

TArray<UListDataObject_Base*> UListDataObject_Bool::GetAllChildListData() const
{
	// Retorna todos os filhos da coleção.
	return ChildListDataArray;
}

bool UListDataObject_Bool::HasAnyChildListData() const
{
	// Retorna se existem filhos registrados.
	return !ChildListDataArray.IsEmpty();
}

#pragma once

/* Namespace helper centralizado para acesso à String Table do Frontend.
 * Evita repetir o caminho do asset e o cast de FTextKey. */
namespace FFrontendLocHelper
{
	/* Busca e retorna um FText a partir de uma chave na String Table ST_Frontend. */
	static FText GetTableTextByKey(const FString& InKey)
	{
		return  FText::FromStringTable(FName("/Game/UI/StringTables/ST_Frontend.ST_Frontend"), FTextKey(InKey));
	}
	

};

// Namespace helper centralizado para modificar a formatação de strings.
namespace FFrontendFormatCase
{
	// Converte a primeira letra de uma string em maiuscula
	static FString UpperFirst(FString InString)
	{
		if (!InString.IsEmpty())
		{
			InString[0] = FChar::ToUpper(InString[0]);
			
			return InString;
		}
		
		return FString();
	}
}

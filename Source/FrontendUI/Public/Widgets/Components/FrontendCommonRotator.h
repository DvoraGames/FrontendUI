// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonRotator.h"
#include "FrontendCommonRotator.generated.h"

/**
* UFrontendCommonRotator
*
* Extensão do UCommonRotator - adiciona utilitário para selecionar
* uma opção pelo texto em vez de pelo índice, com fallback visual
* caso o valor não exista na lista de opções.
*/
UCLASS(Abstract, BlueprintType, meta=(DisableNaiveTick))
class FRONTENDUI_API UFrontendCommonRotator : public UCommonRotator
{
	GENERATED_BODY()
	
public:
	/* Seleciona a opção pelo texto. Se encontrar, atualiza o índice interno e dispara eventos, se não encontrar, força 
	 * o texto visualmente como fallback. */
	void SetSelectedOptionByText(const FText& InTextOption);
	
};

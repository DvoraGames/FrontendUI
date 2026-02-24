// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonRotator.h"
#include "FrontendCommonRotator.generated.h"

/*
 * Extensão do CommonRotator para adicionar utilitários customizados.
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNaiveTick))
class FRONTENDUI_API UFrontendCommonRotator : public UCommonRotator
{
	GENERATED_BODY()
	
public:
	// Seleciona a opção pelo texto, atualizando o índice interno, se a opção não existir, força o texto visualmente (fallback).
	void SetSelectedOptionByText(const FText& InTextOption);
	
};

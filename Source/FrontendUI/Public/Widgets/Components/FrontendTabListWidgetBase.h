// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "FrontendTabListWidgetBase.generated.h"

class UFrontendCommonButtonBase;

/**
* UFrontendTabListWidgetBase
*
* TabList customizado do Frontend - herda de UCommonTabListWidgetBase e
* adiciona suporte a registro de abas via código, aplicando texto nos
* botões via UFrontendCommonButtonBase.
*
* Configurado no Blueprint filho com a classe de botão e a quantidade
* de abas placeholder para preview no editor.
*/
UCLASS(Abstract, BlueprintType, meta=(DisableNaiveTick))
		class FRONTENDUI_API UFrontendTabListWidgetBase : public UCommonTabListWidgetBase
{
	GENERATED_BODY()
	
public:
	// Cria e registra um botão para cada aba do Registry com o ID e texto informados.
	void RequestRegisterTab(const FName& InTabID, const FText& InTabDisplayName);
	
private:
	//~ Begin UWidget Interface
#if WITH_EDITOR	
	// Validação de compilação - emite erro se TabButtonEntryWidgetClass não estiver configurado.
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
	//~ End UWidget Interface
	
	// Quantidade de abas placeholder exibidas no editor para preview - mínimo 1, máximo 10.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Frontend Tab List Settings", 
		meta=(AllowPrivateAccess="true", ClampMin="1", ClamMax="10"))
	int32 DebugEditorPreviewTabCount = 3;
	
	// Tipo do botão usado para cada aba - obrigatório, configurado no Blueprint filho.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Frontend Tab List Settings", 
	meta=(AllowPrivateAccess="true"))
	TSubclassOf<UFrontendCommonButtonBase> TabButtonEntryWidgetClass;
};

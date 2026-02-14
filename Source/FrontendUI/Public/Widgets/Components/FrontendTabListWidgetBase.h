// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "FrontendTabListWidgetBase.generated.h"

class UFrontendCommonButtonBase;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNaiveTick))
		class FRONTENDUI_API UFrontendTabListWidgetBase : public UCommonTabListWidgetBase
{
	GENERATED_BODY()
	
public:
	// OptionsScreen chama isso para cada aba do Registry → cria botão da com ID + texto
	void RequestRegisterTab(const FName& InTabID, const FText& InTabDisplayName);
	
private:
	//~ Begin UWidget Interface
#if WITH_EDITOR	
	// Editor validation: garante que setou TabButtonEntryWidgetClass no Blueprint
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
	//~ End UWidget Interface
	
	// Debug: quantas tabs placeholder deve mostrar no editor (3 por padrão)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Frontend Tab List Settings", 
		meta=(AllowPrivateAccess="true", ClampMin="1", ClamMax="10"))
	int32 DebugEditorPreviewTabCount = 3;
	
	// Qual tipo de botão usar para cada aba
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Frontend Tab List Settings", 
	meta=(AllowPrivateAccess="true"))
	TSubclassOf<UFrontendCommonButtonBase> TabButtonEntryWidgetClass;
};

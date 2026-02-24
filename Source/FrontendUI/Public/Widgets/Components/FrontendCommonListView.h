// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonListView.h"
#include "FrontendCommonListView.generated.h"

class UDataAsset_DataListEntryMapping;

/**
 * Frontend List View que mapeia automaticamente Data Objects para Entry Widgets baseado em Data Asset de configuração.
 * Herda de UCommonListView e sobrescreve a geração de widgets para suportar múltiplos tipos de entradas dinamicamente.
 */
UCLASS()
class FRONTENDUI_API UFrontendCommonListView : public UCommonListView
{
	GENERATED_BODY()
	
protected:
	//~ Begin UCommonListView Interface
	/**
	 * Sobrescrita para gerar widgets de configuração no mapeamento de DataListEntryMapping.
	 * Usa o widget configurado para o tipo específico do Item, ou fallback para o comportamento padrão.
	*/
	virtual  UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;
	//~ End UCommonListView Interface
	
private:
	//~ Begin UWidget Interface
#if WITH_EDITOR	
	// Editor validation: garante que setou DataListEntryMapping no Blueprint 
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
	//~ End UWidget Interface
	
	// Data Asset que mapeia cada tipo de dado da lista ao widget visual correspondente.
	UPROPERTY(EditAnywhere, Category = "Frontend List View Settings")
	UDataAsset_DataListEntryMapping* DataListEntryMapping;
	
};

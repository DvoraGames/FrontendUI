// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonListView.h"
#include "FrontendCommonListView.generated.h"

class UDataAsset_DataListEntryMapping;

/**
* UFrontendCommonListView
*
* ListView customizada do Frontend - herda de UCommonListView e sobrescreve
* a geração de Entry Widgets para mapear automaticamente cada tipo de DataObject
* ao seu widget visual correspondente via DataAsset de configuração.
*
* O mapeamento é configurado no Blueprint filho através do DataListEntryMapping.
*/
UCLASS()
class FRONTENDUI_API UFrontendCommonListView : public UCommonListView
{
	GENERATED_BODY()
	
protected:
	//~ Begin UCommonListView Interface
	/* Sobrescrito para gerar o Entry Widget por tipo de DataObject. Mapeado no DataListEntryMapping — se não encontrar, 
	 * usa o widget padrão da lista. */
	virtual  UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;
	//~ End UCommonListView Interface
	
private:
	//~ Begin UWidget Interface
#if WITH_EDITOR	
	// Validação de compilação - emite erro se DataListEntryMapping não estiver configurado no Blueprint.
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
	//~ End UWidget Interface
	
	// Data Asset que mapeia cada tipo de DataObject ao seu Entry Widget visual correspondente.
	UPROPERTY(EditAnywhere, Category = "Frontend List View Settings")
	UDataAsset_DataListEntryMapping* DataListEntryMapping;
	
};

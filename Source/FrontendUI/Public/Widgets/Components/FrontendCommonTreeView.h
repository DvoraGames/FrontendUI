// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonTreeView.h"
#include "Widgets/Options/Widget_OptionsScreen.h"
#include "FrontendCommonTreeView.generated.h"

class UListDataObject_Base;
class UDataAsset_DataListEntryMapping;
/**
 * 
 */
UCLASS()
class FRONTENDUI_API UFrontendCommonTreeView : public UCommonTreeView
{
	GENERATED_BODY()
	
protected:
	//~ Begin UCommonListView Interface
	/* Sobrescrito para gerar o Entry Widget por tipo de DataObject. Mapeado no DataListEntryMapping — se não encontrar, 
	 * usa o widget padrão da lista. */
	virtual  UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;
	//~ End UCommonListView Interface
	
public:
	// Define os itens iniciais que serão usados como source do TreeView.
	void SetTreeViewItems(const TArray<UListDataObject_Base*>& InTreeItems);

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

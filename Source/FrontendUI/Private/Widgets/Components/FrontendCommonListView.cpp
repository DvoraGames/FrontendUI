// DvoraGames All Rights Reserved


#include "Widgets/Components/FrontendCommonListView.h"

#include "Editor/WidgetCompilerLog.h"
#include "Widgets/Options/DataAsset_DataListEntryMapping.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"

// Gera o widget da linha de configuração para um item da lista.
UUserWidget& UFrontendCommonListView::OnGenerateEntryWidgetInternal(UObject* Item, 
	TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	// No editor (design time), usa o comportamento padrão sem mapeamento customizado
	if (IsDesignTime())
	{
		return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	}
	
	// Consulta o Data Asset para encontrar o widget correspondente ao tipo do item
	if (TSubclassOf<UWidget_ListEntry_Base> FoundWidgetClass = 
		DataListEntryMapping->FindEntryWidgetClassByDataObject(CastChecked<UListDataObject_Base>(Item)))
	{
        // Widget encontrado no mapeamento: gera a linha de configuração da lista com o widget correto
		return GenerateTypedEntry<UWidget_ListEntry_Base>(FoundWidgetClass, OwnerTable);
	}
	
	// Fallback: se não encontrou no mapeamento, usa o widget padrão da lista
	return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
}

// EDITOR VALIDATION (só compila se tiver TabButtonEntryWidgetClass setado)
#if WITH_EDITOR	
void UFrontendCommonListView::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);
	
	if (!DataListEntryMapping)
	{   
		// Emite erro de compilação informando que o Data Asset é obrigatório
		CompileLog.Error(FText::FromString(
			TEXT("The variable DataListEntryMapping hasn't valid data asset assigned ") +
			GetClass()->GetName() + 
			TEXT(" needs a valid data asset to function properly")
			));
	}
}
#endif

// DvoraGames All Rights Reserved


#include "Widgets/Components/FrontendTabListWidgetBase.h"
#include "Editor/WidgetCompilerLog.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"


void UFrontendTabListWidgetBase::RequestRegisterTab(const FName& InTabID, const FText& InTabDisplayName)
{
	// Delega pro CommonTabListWidgetBase: cria/registra o botão usando TabButtonEntryWidgetClass
	RegisterTab(InTabID, TabButtonEntryWidgetClass, nullptr);

	if (UFrontendCommonButtonBase* FoundButton = Cast<UFrontendCommonButtonBase>(GetTabButtonBaseByID(InTabID)))
	{
		// Pega o botão criado e seta o texto ("Gameplay", "Audio", etc)
		FoundButton->SetButtonText(InTabDisplayName);
	}
}

// EDITOR VALIDATION (só compila se tiver TabButtonEntryWidgetClass setado)
#if WITH_EDITOR	
void UFrontendTabListWidgetBase::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);
	
	if (!TabButtonEntryWidgetClass)
	{
		CompileLog.Error(FText::FromString(
			TEXT("The Variable TabButtonEntryWidgetClass has no valid entry specified") + 
			GetClass()->GetName() + 
			TEXT(" needs a valid entry widget class to function properly")
			));
	}
}
#endif

// DvoraGames All Rights Reserved


#include "Widgets/Components/FrontendTabListWidgetBase.h"
#include "Editor/WidgetCompilerLog.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"


void UFrontendTabListWidgetBase::RequestRegisterTab(const FName& InTabID, const FText& InTabDisplayName)
{
	// Delega ao CommonTabListWidgetBase a criação e registro do botão da aba com a classe configurada
	RegisterTab(InTabID, TabButtonEntryWidgetClass, nullptr);

	// Busca o botão da aba recém-criado
	if (UFrontendCommonButtonBase* FoundButton = Cast<UFrontendCommonButtonBase>(GetTabButtonBaseByID(InTabID)))
	{
		// Aplica o texto da aba recem criada (ex: "Gameplay", "Audio")
		FoundButton->SetButtonText(InTabDisplayName);
	}
}

// EDITOR VALIDATION (só compila se tiver TabButtonEntryWidgetClass setado)
#if WITH_EDITOR	
void UFrontendTabListWidgetBase::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);
	
	// Emite erro de compilação se a classe de botão da aba não estiver configurada no Blueprint filho
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

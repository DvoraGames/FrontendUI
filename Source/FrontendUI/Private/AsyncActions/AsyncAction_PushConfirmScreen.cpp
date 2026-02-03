// DvoraGames All Rights Reserved


#include "AsyncActions/AsyncAction_PushConfirmScreen.h"

#include "Subsystems/FrontendUISubsystem.h"

UAsyncAction_PushConfirmScreen* UAsyncAction_PushConfirmScreen::PushConfirmScreen(
	const UObject* WorldContextObject,
	EConfirmScreenType ConfirmScreenType, 
	FText InScreenTitle, FText InScreenMessage
	)
{
	// Verifica se o GEngine é válido (necessário para obter o World)
	if (GEngine)
	{
		// Obtém o World a partir do WorldContextObject, retorna null se falhar (sem crash)
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			// Cria uma nova instância da Async Action usando NewObject
			UAsyncAction_PushConfirmScreen* Node = NewObject<UAsyncAction_PushConfirmScreen>();
			
			Node->CachedOwningWorld = World;				// Armazena o World como ponteiro fraco para uso posterior
			Node->CachedScreenType = ConfirmScreenType;		// Armazena o tipo de modal (Ok/YesNo/OkCancel)
			Node->CachedScreenTitle = InScreenTitle;		// Armazena o título do modal
			Node->CachedScreenMessage = InScreenMessage;    // Armazena a mensagem do modal
			
			// Registra a Async Action com o GameInstance para gerenciamento de lifecycle
			Node->RegisterWithGameInstance(World);
			
			// Retorna o nó criado para ser usado no Blueprint
			return Node;
		}
	}
	
	// Retorna nullptr se GEngine ou World forem inválidos
	return nullptr;
}

void UAsyncAction_PushConfirmScreen::Activate()
{
	Super::Activate();
	
    // Obtém o FrontendUISubsystem do World armazenando o temporariamente
	UFrontendUISubsystem* FrontendUISubsystem = UFrontendUISubsystem::GetFrontendSubsystem(CachedOwningWorld.Get());
	
	// Solicita ao subsystem para criar e exibir o modal de confirmação de forma assíncrona
	FrontendUISubsystem->PushConfirmScreenToModalStackAsync(
		CachedScreenType,		// Tipo do modal (Ok/YesNo/OkCancel)
		CachedScreenTitle,		// Título exibido no modal
		CachedScreenMessage,	// Mensagem exibida no modal
		// Lambda callback executado quando o usuário clica em um botão
		[this](EConfirmScreenButtonType ClickedButtonType)
		{
			// Dispara o Event Dispatcher para notificar Blueprints sobre o botão clicado
			OnButtonClicked.Broadcast(ClickedButtonType);
			
			// Marca a async action como pronta para destruição, liberando recursos
			SetReadyToDestroy();
		}
		);
}

// DvoraGames All Rights Reserved


#include "AsyncActions/AsyncAction_PushConfirmScreen.h"

#include "Subsystems/FrontendUISubsystem.h"

UAsyncAction_PushConfirmScreen* UAsyncAction_PushConfirmScreen::PushConfirmScreen(
	const UObject* WorldContextObject,
	EConfirmScreenType ConfirmScreenType, 
	FText InScreenTitle, FText InScreenMessage
	)
{
	// Verifica se a engine global está disponível.
	if (GEngine)
	{
		// Loga o erro sem crash se o World não for encontrado
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
            // Cria a instância da AsyncAction — ela gerencia toda a operação assíncrona
			UAsyncAction_PushConfirmScreen* Node = NewObject<UAsyncAction_PushConfirmScreen>();
			
			// Cacheia todos os parâmetros recebidos para uso posterior no Activate()
			Node->CachedOwningWorld = World;
			Node->CachedScreenType = ConfirmScreenType;
			Node->CachedScreenTitle = InScreenTitle;
			Node->CachedScreenMessage = InScreenMessage;
			
            // Registra com a GameInstance para garantir o gerenciamento correto de lifetime
			Node->RegisterWithGameInstance(World);
			
			// Retorna a instância criada.
			return Node;
		}
	}
	
	// Retorna nullptr se GEngine ou World forem inválidos
	return nullptr;
}

void UAsyncAction_PushConfirmScreen::Activate()
{
	Super::Activate();
	
    // Obtém o FrontendUISubsystem do World cacheado
	UFrontendUISubsystem* FrontendUISubsystem = UFrontendUISubsystem::Get(CachedOwningWorld.Get());
	
	// Solicita ao subsystem a criação e exibição do modal de forma assíncrona
	FrontendUISubsystem->PushConfirmScreenToModalStackAsync(
		CachedScreenType,		// Tipo do modal (Ok/YesNo/OkCancel)
		CachedScreenTitle,		// Título exibido no modal
		CachedScreenMessage,	// Mensagem exibida no modal
		// Lambda callback executado quando o usuário clica em um botão
		[this](EConfirmScreenButtonType ClickedButtonType)
		{
			// Dispara delegate para notificar Blueprints sobre o botão clicado
			OnButtonClicked.Broadcast(ClickedButtonType);
			
			// Finaliza a AsyncAction — libera memória automaticamente
			SetReadyToDestroy();
		}
		);
}

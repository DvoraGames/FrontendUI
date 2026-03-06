// DvoraGames All Rights Reserved


#include "AsyncActions/AsyncAction_PushSoftWidget.h"
#include "Subsystems/FrontendUISubsystem.h"
#include "Widgets/Widget_ActivatableBase.h"

UAsyncAction_PushSoftWidget* UAsyncAction_PushSoftWidget::PushSoftWidget(
	const UObject* WorldContextObject,
	APlayerController* OwningPlayerController, 
	TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass,
	FGameplayTag InWidgetStackTag, 
	bool bFocusOnPushedWidget)
{
	// Garante em dev que a soft reference não é nula — crasha com log se inválida
	checkf(!InSoftWidgetClass.IsNull(), TEXT("PushSoftWidgetToStack has a null Soft Widget class"));
	
	// Verifica se a engine global está disponível.
	if (GEngine)
	{
		// Loga o erro sem crash se o World não for encontrado
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			// Cria a instância da AsyncAction — ela gerencia toda a operação assíncrona
			UAsyncAction_PushSoftWidget* Node = NewObject<UAsyncAction_PushSoftWidget>();
			
			// Cacheia todos os parâmetros recebidos para uso posterior no Activate()
			Node->CachedOwningWorld = World;
			Node->CachedOwningPC = OwningPlayerController;
			Node->CachedSoftWidgetClass = InSoftWidgetClass;
			Node->CachedWidgetStackTag = InWidgetStackTag;
			Node->bCachedFocusPushedWidget = bFocusOnPushedWidget;
			
			// Registra com a GameInstance para garantir o gerenciamento correto de lifetime
			Node->RegisterWithGameInstance(World);
			
			// Retorna a instância criada.
			return Node;
		}
	}
	
	// Retorna nullptr se GEngine ou World forem inválidos
	return nullptr;
}

void UAsyncAction_PushSoftWidget::Activate()
{
	// Obtém o FrontendUISubsystem do World cacheado
	UFrontendUISubsystem* FrontendUISubsystem = UFrontendUISubsystem::Get(CachedOwningWorld.Get());
	
	// Inicia o push assíncrono — carrega CachedSoftWidgetClass em background e insere no stack ao terminar
	FrontendUISubsystem->PushSoftWidgetToStackAsync(CachedWidgetStackTag, CachedSoftWidgetClass,
		[this](EAsyncPushWidgetState InPushState, UWidget_ActivatableBase* PushedWidget)
		{
			// Callback disparado em 2 estágios pelo FrontendUISubsystem:
			switch (InPushState)
			{	
			// ESTÁGIO 1: Widget criado, mas ainda NÃO inserido na stack
			case EAsyncPushWidgetState::OnCreatedBeforePush:
				// Define PlayerController dono antes do push final
				PushedWidget->SetOwningPlayer(CachedOwningPC.Get());
				
				// Dispara delegate no BP "antes do push" - lógica de setup
				OnWidgetCreatedBeforePush.Broadcast(PushedWidget);
				
				break;
				
			// ESTÁGIO 2: Widget inserido, ativado e visivel na stack
			case EAsyncPushWidgetState::AfterPush:
				
				// Dispara delegate no BP "After Push" - lógica de pós-push, quando o widget já está ativo na stack
				AfterPush.Broadcast(PushedWidget);
				
				// Verifica se foi solicitado no PushSoftWidget() o foco automático.
				if (bCachedFocusPushedWidget)
				{
					// Foca no widget filho definido como alvo de foco no Blueprint do widget
					if (UWidget* WidgetToFocus = PushedWidget->GetDesiredFocusTarget())
					{
						WidgetToFocus->SetFocus();
					}
				}
				
				// FINALIZA AsyncAction - libera memória automaticamente
				SetReadyToDestroy();
				break;
			default:
				break;
			}
		}
		);

}

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
	// Verifica se a soft reference do widget não é nula. Se for, causa crash em desenvolvimento com mensagem de erro.	
	checkf(!InSoftWidgetClass.IsNull(), TEXT("PushSoftWidgetToStack has a null Soft Widget class"));
	
	// Verifica se a engine global está disponível.
	if (GEngine)
	{
		// Obtém o UWorld* a partir do WorldContextObject. EGetWorldErrorMode::LogAndReturnNull faz log de erro, mas não causa crash se falhar.
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			// Cria uma nova instância da Async Action usando NewObject (construtor de UObject do Unreal).
			// Essa instância será o objeto que gerencia toda a operação assíncrona.
			UAsyncAction_PushSoftWidget* Node = NewObject<UAsyncAction_PushSoftWidget>();
			
			// Faz o cache de todos os parâmetros recebidos para o uso posterior na execução assíncrona.
			/* Esses valores são acessados pelo sistema interno da AsyncAction quando o widget
			for carregado da soft reference e adicionado à stack do player especificada.*/
			Node->CachedOwningWorld = World;
			Node->CachedOwningPC = OwningPlayerController;
			Node->CachedSoftWidgetClass = InSoftWidgetClass;
			Node->CachedWidgetStackTag = InWidgetStackTag;
			Node->bCachedFocusPushedWidget = bFocusOnPushedWidget;
			
			// Registra a async action com a Game Instance para gerenciamento de lifetime.
			// Isso garante que a action seja destruída corretamente quando a Game Instance terminar.
			Node->RegisterWithGameInstance(World);
			
			// Retorna a instância criada.
			return Node;
		}
	}
	
	return nullptr;
}

void UAsyncAction_PushSoftWidget::Activate()
{
	// Obtém FrontendUISubsystem do mundo cached (CommonUI sistema de UI)
	UFrontendUISubsystem* FrontendUISubsystem = UFrontendUISubsystem::GetFrontendSubsystem(CachedOwningWorld.Get());
	
	// Inicia push ASSÍNCRONO do widget na stack específica usando sistema customizado
	// Carrega CachedSoftWidgetClass em background → push na CachedWidgetStackTag do CachedOwningPC
	FrontendUISubsystem->PushSoftWidgetToStackAsync(CachedWidgetStackTag, CachedSoftWidgetClass,
		[this](EAsyncPushWidgetState InPushState, UWidget_ActivatableBase* PushedWidget)
		{
			// Callback disparado em 2 estágios pelo FrontendUISubsystem:
			switch (InPushState)
			{	
			case EAsyncPushWidgetState::OnCreatedBeforePush:
				// ESTÁGIO 1: Widget criado, mas AINDA NÃO "pushado" na stack
				// Define PlayerController dono antes do push final
				PushedWidget->SetOwningPlayer(CachedOwningPC.Get());
				
				// Dispara delegate BP "antes do push" - conecte lógica de setup aqui
				OnWidgetCreatedBeforePush.Broadcast(PushedWidget);
				
				break;
			case EAsyncPushWidgetState::AfterPush:
				// ESTÁGIO 2: Widget "pushado", ativado e VISÍVEL na stack
				AfterPush.Broadcast(PushedWidget);
				
				// Foco automático se solicitado no PushSoftWidget()
				if (bCachedFocusPushedWidget)
				{
					// Busca widget filho que deve receber foco (definido no Blueprint do widget)
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

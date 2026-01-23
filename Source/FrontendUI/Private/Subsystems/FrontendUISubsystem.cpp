// DvoraGames All Rights Reserved


#include "Subsystems/FrontendUISubsystem.h"

#include "Engine/AssetManager.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "Widgets/Widget_PrimaryLayout.h"

// Função usada para "pegar" o Subsystem do Frontend no c++
UFrontendUISubsystem* UFrontendUISubsystem::GetFrontendSubsystem(const UObject* WorldContextObject)
{
	// Verifica se GEngine é valida
	if (GEngine)
	{
		// Obtém o World (mapa atual) a partir do objeto de contexto, caso não encontre lança um erro fatal
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
		
		// Retorna a instância do subsystem UFrontendUISubsystem do GameInstance atual
		return UGameInstance::GetSubsystem<UFrontendUISubsystem>(World->GetGameInstance());
	}
	
	return nullptr;
}

bool UFrontendUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	// Converte e Verifica se NÃO é uma instância de servidor dedicado, caso a conversão falhe, causará um crash
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		// Busca todas as classes derivadas da classe deste objeto, neste caso o subsistema
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);
		
		// Só cria o subsistema se o TArray estiver vazio, ou seja, se não encontrou uma classe derivada desta
		return FoundClasses.IsEmpty();
	}
	
	// Retorna false se for um servidor dedicado, ou seja, nào cria o subsistema da UI
	return false;
}

void UFrontendUISubsystem::RegisterCreatedPrimaryLayoutWidget(UWidget_PrimaryLayout* InCreatedWidget)
{
	// Verifica se o parametro InCreatedWidget é valido, caso contrario crasha o jogo
	check(InCreatedWidget);
	
	// Atribui InCreatedWidget a variavel CreatedPrimaryLayout
	CreatedPrimaryLayout = InCreatedWidget;
}

void UFrontendUISubsystem::PushSoftWidgetToStackAsync(
	const FGameplayTag& InWidgetStackTag, 
	TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass,
	TFunction<void(EAsyncPushWidgetState, UWidget_ActivatableBase*)> AsyncPushStateCallback
	)
{
	// Verifica se o parametro InSoftWidget Não é Nulo, caso contrario crasha
	check(!InSoftWidgetClass.IsNull());
	
	// Solicita carregamento assíncrono do asset representado por InSoftWidgetClass.
	/* Isso significa que o asset (classe do widget) será carregado em segundo plano e o código dentro do lambda rodará apenas após o carregamento terminar. */
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(InSoftWidgetClass.ToSoftObjectPath(), 
		FStreamableDelegate::CreateLambda(
			[InSoftWidgetClass, this, InWidgetStackTag, AsyncPushStateCallback]()
			{
				/* Obtém a classe carregada do asset. Após o carregamento assíncrono, esse ponteiro será válido se o asset foi carregado corretamente. */
				UClass* LoadedWidgetClass = InSoftWidgetClass.Get();
				
				/* Verifica se a classe foi carregada com sucesso e se o layout primário foi criado antes de continuar, caso contrario crasha. */
				check(LoadedWidgetClass && CreatedPrimaryLayout);
				
				/* Busca o container (stack) de widgets pelo tag informado. Usado para organizar e gerenciar múltiplos widgets de forma dinâmica. */
				UCommonActivatableWidgetContainerBase* FoundWidgetStack = CreatedPrimaryLayout->FindWidgetStackByTag(InWidgetStackTag);

				// Adiciona ao stack encontrado um novo widget do tipo UWidget_ActivatableBase. 
				UWidget_ActivatableBase* CreatedWidget = FoundWidgetStack->AddWidget<UWidget_ActivatableBase>(
					LoadedWidgetClass, 
					[AsyncPushStateCallback](UWidget_ActivatableBase& CreatedWidgetInstance)
					{
						// Chama callback informando que o widget foi criado, mas ainda não está no stack. 
						// Serve para executar lógica customizada antes de realmente inserir o widget no container.
						AsyncPushStateCallback(EAsyncPushWidgetState::OnCreatedBeforePush, &CreatedWidgetInstance);
					});
				
				// Chama callback indicando que o widget foi inserido no stack e está pronto para uso.
				AsyncPushStateCallback(EAsyncPushWidgetState::AfterPush, CreatedWidget);
			}
		));
}

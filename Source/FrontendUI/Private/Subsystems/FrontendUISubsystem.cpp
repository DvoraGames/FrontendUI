// DvoraGames All Rights Reserved


#include "Subsystems/FrontendUISubsystem.h"

#include "EnhancedInputSubsystems.h"
#include "FrontendFunctionLibrary.h"
#include "FrontendGameplayTags.h"
#include "Controllers/FrontendPlayerController.h"
#include "Engine/AssetManager.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "Widgets/Widget_ConfirmScreen.h"
#include "Widgets/Widget_PrimaryLayout.h"

// Função usada para "pegar" o Subsystem do Frontend no c++
UFrontendUISubsystem* UFrontendUISubsystem::Get(const UObject* WorldContextObject)
{
	// Verifica se a engine global está disponível.
	if (GEngine)
	{
		// Obtém o World a partir do contexto — erro fatal se não encontrar
		const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
		
		// Retorna a instância do subsystem da GameInstance atual
		return UGameInstance::GetSubsystem<UFrontendUISubsystem>(World->GetGameInstance());
	}
	
	// Retorna nullptr.
	return nullptr;
}

bool UFrontendUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	// Converte Outer para UGameInstance (crasha se falhar) e verifica se NÃO é servidor dedicado
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		// Array para armazenar as subclasses deste subsistem
		TArray<UClass*> FoundClasses;
		
		// Busca subclasses deste subsystem e armazena no array se encontrar
		GetDerivedClasses(GetClass(), FoundClasses);
		
		// Só cria se não houver subclasse dentro do array - evita duplicata com uma versão derivada
		return FoundClasses.IsEmpty();
	}
	
	// Retorna false se for servidor dedicado - não cria o subsystem de UI
	return false;
}

void UFrontendUISubsystem::RegisterCreatedPrimaryLayoutWidget(UWidget_PrimaryLayout* InCreatedWidget)
{
	// Garante que o widget é válido antes de registrar
	check(InCreatedWidget);
	
	// Armazena a referência do PrimaryLayout (InCreatedWidget)
	CreatedPrimaryLayout = InCreatedWidget;
}

void UFrontendUISubsystem::PushSoftWidgetToStackAsync(
	const FGameplayTag& InWidgetStackTag, 
	TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass,
	TFunction<void(EAsyncPushWidgetState, UWidget_ActivatableBase*)> AsyncPushStateCallback
	)
{
	// Garante que a soft reference não é nula
	check(!InSoftWidgetClass.IsNull());
	
	// Solicita carregamento assíncrono - o lambda roda somente após o asset ser carregado
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(InSoftWidgetClass.ToSoftObjectPath(), 
		FStreamableDelegate::CreateLambda(
			[InSoftWidgetClass, this, InWidgetStackTag, AsyncPushStateCallback]()
			{
				// Obtém a classe carregada do asset após o carregamento - será válido se foi carregado corretamente.
				UClass* LoadedWidgetClass = InSoftWidgetClass.Get();
				
				// Garante que a classe foi carregada e o PrimaryLayout existe
				check(LoadedWidgetClass && CreatedPrimaryLayout);
				
				// Busca o container stack pela tag informada
				UCommonActivatableWidgetContainerBase* FoundWidgetStack = CreatedPrimaryLayout->FindWidgetStackByTag(InWidgetStackTag);

				// Adiciona o widget a stack e executa o callback antes do push
				UWidget_ActivatableBase* CreatedWidget = FoundWidgetStack->AddWidget<UWidget_ActivatableBase>(
					LoadedWidgetClass, 
					[AsyncPushStateCallback](UWidget_ActivatableBase& CreatedWidgetInstance)
					{
						// Estágio 1: widget criado, ainda não inserido - dispara callback para setup
						AsyncPushStateCallback(EAsyncPushWidgetState::OnCreatedBeforePush, &CreatedWidgetInstance);
					});
				
				// Estágio 2: widget inserido no stack e ativo
				AsyncPushStateCallback(EAsyncPushWidgetState::AfterPush, CreatedWidget);
			}
		));
}

void UFrontendUISubsystem::PushConfirmScreenToModalStackAsync(
	EConfirmScreenType InScreenType,
	const FText& InScreenTitle, const FText& InScreenMsg,
	TFunction<void(EConfirmScreenButtonType)> ButtonClickedCallback)
{
	// Variável para receber o pacote de dados do modal
	UConfirmScreenInfoObject* CreatedInfoObject = nullptr;
	
	// Cria o pacote correto baseado no tipo de modal solicitado
	switch (InScreenType)
	{
	case EConfirmScreenType::Ok:
		// Cria Modal do tipo Ok
		CreatedInfoObject = UConfirmScreenInfoObject::CreateOkScreen(InScreenTitle, InScreenMsg);
		break;
	case EConfirmScreenType::YesNo:
		// Cria Modal do tipo YesNo
		CreatedInfoObject = UConfirmScreenInfoObject::CreateYesNoScreen(InScreenTitle, InScreenMsg);
		break;
	case EConfirmScreenType::OkCancel:
		// Cria Modal do tipo OkCancel
		CreatedInfoObject = UConfirmScreenInfoObject::CreateOkCancelScreen(InScreenTitle, InScreenMsg);
		break;
	case EConfirmScreenType::Unknown:
		break;
	default:
		break;
	}
	
	// Garante que o pacote foi criado com sucesso
	check(CreatedInfoObject);
	
	// Faz o push assíncrono do ConfirmScreen no stack Modal
	PushSoftWidgetToStackAsync(
		FrontendGameplayTags::Frontend_WidgetStack_Modal,
		UFrontendFunctionLibrary::GetFrontendSoftWidgetClassByTag(FrontendGameplayTags::Frontend_Widget_ConfirmScreen),
		[CreatedInfoObject, ButtonClickedCallback](EAsyncPushWidgetState InPushState, UWidget_ActivatableBase* PushedWidget)
		{
			// Estágio 1: antes do push - inicializa o modal com os dados e conecta o callback
			if (InPushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				// Tenta converter (Cast) o PushedWidget em UWidget_ConfirmScreen, crasha caso falhe
				UWidget_ConfirmScreen* CreatedConfirmScreen = CastChecked<UWidget_ConfirmScreen>(PushedWidget);
				
				// Inicializa o modal: aplica dados (title/botões) e conecta callback de resultado
				CreatedConfirmScreen->InitConfirmScreen(CreatedInfoObject, ButtonClickedCallback);
			}
		}
		);
}

void UFrontendUISubsystem::AddGlobalInputMappingContext(AFrontendPlayerController* FrontendPC, int32 Priority)
{
	// Garante que o PlayerController é válido
	checkf(FrontendPC, TEXT("Player Controller not found"));
	
	// Verifica se o GlobalIMC foi configurado no FrontendPC.
	if (FrontendPC->GetGlobalIMC())
	{
        // Obtém o LocalPlayer do PlayerController e verifica se é válido
		if (ULocalPlayer* LocalPlayer = FrontendPC->GetLocalPlayer())
		{
			// Obtém o Enhanced Input Local Player Subsystem para gerenciar os Input Mapping Contexts
			if (UEnhancedInputLocalPlayerSubsystem* EIS = LocalPlayer->
						GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				// Adiciona o IMC ao Enhanced Input com a prioridade especificada
				EIS->AddMappingContext(FrontendPC->GetGlobalIMC(), Priority);
			}
		}
	}

}

void UFrontendUISubsystem::RemoveGlobalInputMappingContext(AFrontendPlayerController* FrontendPC)
{
    // Garante que o PlayerController é válido
	checkf(FrontendPC, TEXT("Player Controller not found"));
	
	// Verifica se o GlobalIMC foi configurado no FrontendPC.
	if (FrontendPC->GetGlobalIMC())
	{
		// Obtém o LocalPlayer do PlayerController e verifica se é válido
		if (ULocalPlayer* LocalPlayer = FrontendPC->GetLocalPlayer())
		{
			// Obtém o Enhanced Input Local Player Subsystem para gerenciar os Input Mapping Contexts
			if (UEnhancedInputLocalPlayerSubsystem* EIS = LocalPlayer->
						GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				// Remove o IMC do Enhanced Input
				EIS->RemoveMappingContext(FrontendPC->GetGlobalIMC());
			}
		}
	}
}



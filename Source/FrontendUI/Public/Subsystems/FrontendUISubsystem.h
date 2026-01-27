// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "FrontendUISubsystem.generated.h"

class UFrontendCommonButtonBase;
class UWidget_PrimaryLayout;
struct FGameplayTag;
class UWidget_ActivatableBase;

// Enum para antes do Push e depois do Push do Widget
enum class EAsyncPushWidgetState : uint8
{
	OnCreatedBeforePush,
	AfterPush
};

// Delegate para notificar atualizações da descrição de botões no sistema Frontend.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnButtonDescriptionTextUpdateDelegate, // Nome do delegate
	UFrontendCommonButtonBase*,				// Tipo do parametro
	BroadcastingButton,						// Nome do parametro
	FText,									// Tipo do parametro 2
	Description								// Nome do parametro 2
	);

UCLASS()
class FRONTENDUI_API UFrontendUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// Função usada para "pegar" o Subsystem do Frontend no c++
	static UFrontendUISubsystem* GetFrontendSubsystem(const UObject* WorldContextObject);
	
	//~ Begin USubsystem Interface
	// Determina se este subsistema deve ser criado
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	//~ End USubsystem Interface
	
	// Função usada para Registrar o PrimaryLayout Widget Criado ao subsystem
	UFUNCTION(BlueprintCallable)
	void RegisterCreatedPrimaryLayoutWidget(UWidget_PrimaryLayout* InCreatedWidget);
	
	// Carrega assincronamente uma classe de Widget e adiciona ao Widget Stack especificado pela Gameplay Tag.
	// Esta função usa Soft Class Ptr para carregar o widget sob demanda (streaming), evitando referências hard.
	// O callback é executado em dois momentos: antes do Push (para inicialização) e após o Push (widget ativo).
	void PushSoftWidgetToStackAsync(const FGameplayTag& InWidgetStackTag, 
		TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass, 
		TFunction<void(EAsyncPushWidgetState, UWidget_ActivatableBase*)> AsyncPushStateCallback);
	
	// Declaração que permite atribuir funções Blueprint a este delegate via Event Dispatcher
	UPROPERTY(BlueprintAssignable)
	FOnButtonDescriptionTextUpdateDelegate OnButtonDescriptionTextUpdate;
private:
	// Propriedade temporaria para armazenar o Widget Primario/Root
	UPROPERTY(Transient)
	UWidget_PrimaryLayout* CreatedPrimaryLayout;
	
};

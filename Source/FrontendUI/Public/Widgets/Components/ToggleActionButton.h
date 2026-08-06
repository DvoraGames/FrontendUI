// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "ToggleActionButton.generated.h"

class UListDataObject_Value;
enum class EToggleActionType : uint8;
class UCommonLazyImage;

/**
 * UToggleActionButton
 *
 * Botão base que representa uma ação de toggle (Expand, Mute, Link) e alterna
 * seu ícone/estado visual conforme o tipo de ação e se está ativa.
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNaiveTick))
class FRONTENDUI_API UToggleActionButton : public UCommonButtonBase
{
	GENERATED_BODY()
	
public:
	// Define o tipo de ação exibida por este botão.
	UFUNCTION(BlueprintCallable, Category="Toggle Action")
	void SetToggleActionType(EToggleActionType InActionType);
	
	// Define se a ação está ativa.
	UFUNCTION(BlueprintCallable, Category="Toggle Action")
	void SetIsActionActive (bool bInActionActive);
	
	// Retorna o tipo de ação atual.
	UFUNCTION(BlueprintPure, Category="Toggle Action")
	EToggleActionType GetActionType() const { return ActionType; }
	
	// Retorna se a ação está ativa atualmente.
	UFUNCTION(BlueprintPure, Category="Toggle Action")
	bool GetIsActionActive() const { return bIsActionActive; }
	
	// Sincroniza tipo, estado e visibilidade do botão com o DataObject fornecido.
	void SetVisibilityByActionType(const UListDataObject_Base* InDataObject);
	
protected:
	//~ Begin UUserWidget Interface
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	//~ End UUserWidget Interface
	
	// Atualiza o ícone e notifica o Blueprint sobre o estado atual do botão.
	UFUNCTION(BlueprintCallable)
	virtual void RefreshVisual();
	
	// Resolve qual ícone usar com base no tipo de ação e no estado ativo.
	virtual TSoftObjectPtr<UTexture2D> ResolveIconByActionType() const;
	
	// Evento em Blueprint chamado após RefreshVisual, para permitir ajustes visuais adicionais.
	UFUNCTION(BlueprintImplementableEvent, Category="Toggle Action", meta=(DisplayName="On Refresh Visual"))
	void BP_OnRefreshVisual(EToggleActionType InActionType, bool bInIsActionActive);

private:
	//** ----------------------------------
	//** Bound Widgets
	//** ----------------------------------
	
	// Imagem interna do botão.
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess="true"))
	UCommonLazyImage* CommonLazyImage_ActionIcon;
	
	
	//** ----------------------------------
	//** Propriedades
	//** ----------------------------------
	
	// Tipo atual da ação exibida.
	UPROPERTY(EditAnywhere, Category="Toggle Action", meta=(AllowPrivateAccess="true"))
	EToggleActionType ActionType = EToggleActionType::None;

	// Estado lógico atual da ação.
	UPROPERTY(EditAnywhere, Category="Toggle Action", meta=(AllowPrivateAccess="true"))
	bool bIsActionActive = false;

	//** ----------------------------------
	//** Icones
	//** ----------------------------------
	
	// Ícone usado para Expand.
	UPROPERTY(EditDefaultsOnly, Category="Toggle Action|Icons", meta=(AllowPrivateAccess="true"))
	TSoftObjectPtr<UTexture2D> Icon_Expand;

	// Ícone usado para Collapse.
	UPROPERTY(EditDefaultsOnly, Category="Toggle Action|Icons", meta=(AllowPrivateAccess="true"))
	TSoftObjectPtr<UTexture2D> Icon_Collapse;

	// Ícone usado para Mute.
	UPROPERTY(EditDefaultsOnly, Category="Toggle Action|Icons", meta=(AllowPrivateAccess="true"))
	TSoftObjectPtr<UTexture2D> Icon_Mute;

	// Ícone usado para Unmute.
	UPROPERTY(EditDefaultsOnly, Category="Toggle Action|Icons", meta=(AllowPrivateAccess="true"))
	TSoftObjectPtr<UTexture2D> Icon_Unmute;

	// Ícone usado para Link.
	UPROPERTY(EditDefaultsOnly, Category="Toggle Action|Icons", meta=(AllowPrivateAccess="true"))
	TSoftObjectPtr<UTexture2D> Icon_Link;

	// Ícone usado para Unlink.
	UPROPERTY(EditDefaultsOnly, Category="Toggle Action|Icons", meta=(AllowPrivateAccess="true"))
	TSoftObjectPtr<UTexture2D> Icon_Unlink;
};

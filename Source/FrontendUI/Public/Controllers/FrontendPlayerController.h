// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "FrontendPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FRONTENDUI_API AFrontendPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	// Propriedade para setar o IMC pelo painel Details
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputMappingContext* GlobalInputMapping_UI;
	
	// Função para retornar o IMC setado
	UFUNCTION(BlueprintPure)
	UInputMappingContext* GetGlobalIMC() const;

protected:
	//~ Begin APlayerController Interface
	virtual void OnPossess(APawn* aPawn) override;
	//~ End APlayerController Interface
	
};

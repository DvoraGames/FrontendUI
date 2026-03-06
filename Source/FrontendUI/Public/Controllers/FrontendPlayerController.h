// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "FrontendPlayerController.generated.h"

/**
* AFrontendPlayerController
*
* Player Controller base do Frontend.
* Responsável por armazenar o Input Mapping Context global da UI e ativar
* a câmera padrão do mapa ao possuir um Pawn.
*/
UCLASS()
class FRONTENDUI_API AFrontendPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	// Input Mapping Context global da UI — configurado no painel Details do Blueprint filho.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputMappingContext* GlobalInputMapping_UI;
	
    // Retorna no BP, o Input Mapping Context global da UI configurado neste Controller.
	UFUNCTION(BlueprintPure)
	UInputMappingContext* GetGlobalIMC() const;

protected:
	//~ Begin APlayerController Interface
	// Chamado ao possuir um Pawn - busca e ativa a câmera padrão do mapa.
	virtual void OnPossess(APawn* aPawn) override;
	//~ End APlayerController Interface
	
};

// DvoraGames All Rights Reserved


#include "Controllers/FrontendPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"

// Função para retornar o IMC setado
UInputMappingContext* AFrontendPlayerController::GetGlobalIMC() const
{
	return GlobalInputMapping_UI;
}

void AFrontendPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	
	// Procura por um Camera Actor com a Tag "DefaultCam" e armazena na variavel FoundCamera
	TArray<AActor*> FoundCameras;
	UGameplayStatics::GetAllActorsOfClassWithTag(this, ACameraActor::StaticClass(), FName("DefaultCam"), FoundCameras);
	
	// Checa se FoundCameras não está vazio
	if (!FoundCameras.IsEmpty())
	{
		// Ativa a camera encontrada
		SetViewTarget(FoundCameras[0]);
	}
}

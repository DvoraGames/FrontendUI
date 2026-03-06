// DvoraGames All Rights Reserved

#include "Controllers/FrontendPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"

UInputMappingContext* AFrontendPlayerController::GetGlobalIMC() const
{
	// Retorna o IMC global da UI configurado neste Controller
	return GlobalInputMapping_UI;
}

void AFrontendPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	
	// Array para armazenar as cameras encontradas no mapa
	TArray<AActor*> FoundCameras;
	
	// Busca todos os Camera Actors no mapa que possuem a tag "DefaultCam" e guarda no array
	UGameplayStatics::GetAllActorsOfClassWithTag(
		this, ACameraActor::StaticClass(), FName("DefaultCam"), FoundCameras);
	
	// Verifica se o array FoundCameras não está vazio
	if (!FoundCameras.IsEmpty())
	{
		// Ativa a primeira câmera encontrada como view target
		SetViewTarget(FoundCameras[0]);
	}
}

// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ListDataObject_String.h"
#include "ListDataObject_StringCarousel.generated.h"

/**
* UListDataObject_Carousel
*
* DataObject para opções de menu do tipo Carrossel.
* Permite navegar ciclicamente (anterior/próximo) entre uma lista de opções pré-definidas.
* Separa o valor interno (FString) do texto traduzível exibido ao jogador (FText).
*/
UCLASS()
class FRONTENDUI_API UListDataObject_StringCarousel : public UListDataObject_String
{
	GENERATED_BODY()
	
public:
	// Atualiza o valor selecionado a partir de um texto - chamado pelo componente visual Carousel.
	void OnCarouselInitiatedValueChange(const FText& InNewSelectedText);

};

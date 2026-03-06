// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "OptionsDataRegistry.generated.h"

class UListDataObject_Base;
class UListDataObject_Collection;

/*
 * "Registry" central: cria e registra todas as abas de opções (Gameplay/Audio/Video/Control)
 * e expõe métodos para a tela de opções obter os itens de cada aba.
 *
 * Obs: Não é singleton de verdade, mas se comporta como um "único ponto de verdade" para essas opções.
 */

/**
* UOptionsDataRegistry
*
* Registry central de opções do Frontend - cria e registra todas as abas
* (Gameplay, Audio, Video, Controls) e seus DataObjects filhos.
*
* Instanciado pela OptionsScreen via lazy initialization e destruído junto com ela.
* Expõe métodos para a tela obter as abas registradas e os itens de cada aba.
*/
UCLASS()
class FRONTENDUI_API UOptionsDataRegistry : public UObject
{
	GENERATED_BODY()
	
	
public:
	// Inicializa o catálogo criando todas as abas e seus itens - chamado pela OptionsScreen logo após criar o Registry.
	void InitOptionsDataRegistry(ULocalPlayer* OwningLocalPlayer);
	
	// Retorna o array com todas as abas registradas - usado pela OptionsScreen para criar os botões do TabList.
	const TArray<UListDataObject_Collection*>& GetRegisteredOptionsTabCollection() const { return RegisteredOptionsTabCollections; }
	
	// Retorna os DataObjects (opções) da aba identificada pelo TabID - usado pela OptionsScreen para popular a ListView.
	TArray<UListDataObject_Base*> GetListSourceItemBySelectedTabID(const FName InSelectedTabID) const;
	
private:
	// Cria e registra a aba Gameplay com suas opções
	void InitGamePlayCollectionTab();
	
	// Cria e registra a aba Audio
	void InitAudioCollectionTab();
	
	// Cria e registra a aba Video
	void InitVideoCollectionTab();
	
	// Cria e registra a aba Controls
	void InitControlCollectionTab();
	
	// Array com as abas registradas - Transient, reconstruído a cada sessão.
	UPROPERTY(Transient)
	TArray<UListDataObject_Collection*> RegisteredOptionsTabCollections;
};

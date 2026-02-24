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
UCLASS()
class FRONTENDUI_API UOptionsDataRegistry : public UObject
{
	GENERATED_BODY()
	
	
public:
	// Esta função é chamada pela Options Screen logo após o objeto UOptionsDataRegistry ser criado
	void InitOptionsDataRegistry(ULocalPlayer* OwningLocalPlayer);
	
	// OptionsScreen itera aqui pra pegar Gameplay/Audio/Video/Control e criar botões das abas
	const TArray<UListDataObject_Collection*>& GetRegisteredOptionsTabCollection() const { return RegisteredOptionsTabCollections; }
	
	// Retorna os itens (entradas) da aba selecionada, identificada pelo TabID.
	TArray<UListDataObject_Base*> GetListSourceItemBySelectedTabID(const FName InSelectedTabID) const;
	
private:
	// Funções privadas que criam cada aba específica e adicionam no array
	void InitGamePlayCollectionTab();
	void InitAudioCollectionTab();
	void InitVideoCollectionTab();
	void InitControlCollectionTab();
	
	// Array final com ponteiros para as 4 abas: [Gameplay, Audio, Video, Controls]
	UPROPERTY(Transient)
	TArray<UListDataObject_Collection*> RegisteredOptionsTabCollections;
};

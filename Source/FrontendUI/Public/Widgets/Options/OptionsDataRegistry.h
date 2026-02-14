// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "OptionsDataRegistry.generated.h"

class UListDataObject_Collection;

/*
 * SINGLETON-LIKE: centraliza criação de TODAS as abas e suas sub-paginas
*/
UCLASS()
class FRONTENDUI_API UOptionsDataRegistry : public UObject
{
	GENERATED_BODY()
	
	
public:
	// Esta função é chamada pela Options Screen logo após o objeto UOptionsDataRegistry ser criado
	void InitOptionsDataRegistry(ULocalPlayer* OwningLocalPlayer);
	
	// OptionsScreen itera aqui pra pegar Gameplay/Audio/Video/Control e criar botões das abas
	const TArray<UListDataObject_Collection*>& GetRegisteredOptionsTabCollection() const { return RegisteredOptionsTabCollection; }
	
private:
	// Funções privadas que criam cada aba específica e adicionam no array
	void InitGamePlayCollectionTab();
	void InitAudioCollectionTab();
	void InitVideoCollectionTab();
	void InitControlCollectionTab();
	
	// Array final com ponteiros para as 4 abas: [Gameplay, Audio, Video, Controls]
	UPROPERTY(Transient)
	TArray<UListDataObject_Collection*> RegisteredOptionsTabCollection;
};

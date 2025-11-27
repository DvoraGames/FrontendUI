// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FrontendUISubsystem.generated.h"

class UWidget_PrimaryLayout;

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
	
	UFUNCTION(BlueprintCallable)
	void RegisterCreatedPrimaryLayoutWidget(UWidget_PrimaryLayout* InCreatedWidget);
	
private:
	// Propriedade temporaria para armazenar o Widget Primario/Root
	UPROPERTY(Transient)
	UWidget_PrimaryLayout* CreatedPrimaryLayout;
	
};

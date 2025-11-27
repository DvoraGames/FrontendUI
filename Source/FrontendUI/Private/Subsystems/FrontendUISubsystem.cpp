// DvoraGames All Rights Reserved


#include "Subsystems/FrontendUISubsystem.h"

// Função usada para "pegar" o Subsystem do Frontend no c++
UFrontendUISubsystem* UFrontendUISubsystem::GetFrontendSubsystem(const UObject* WorldContextObject)
{
	// Verifica se GEngine é valida
	if (GEngine)
	{
		// Obtém o World (mapa atual) a partir do objeto de contexto, caso não encontre lança um erro fatal
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
		
		// Retorna a instância do subsystem UFrontendUISubsystem do GameInstance atual
		return UGameInstance::GetSubsystem<UFrontendUISubsystem>(World->GetGameInstance());
	}
	
	return nullptr;
}

bool UFrontendUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	// Converte e Verifica se NÃO é uma instância de servidor dedicado, caso a conversão falhe, causará um crash
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		// Busca todas as classes derivadas da classe deste objeto, neste caso o subsistema
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);
		
		// Só cria o subsistema se o TArray estiver vazio, ou seja, se não encontrou uma classe derivada desta
		return FoundClasses.IsEmpty();
	}
	
	// Retorna false se for um servidor dedicado, ou seja nào cria o subsistema da UI
	return false;
}

void UFrontendUISubsystem::RegisterCreatedPrimaryLayoutWidget(UWidget_PrimaryLayout* InCreatedWidget)
{
	check(InCreatedWidget);
	
	CreatedPrimaryLayout = InCreatedWidget;
}

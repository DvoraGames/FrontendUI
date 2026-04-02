// DvoraGames All Rights Reserved


#include "Widgets/Components/FrontendCommonAnimatedSwitcher.h"

// Inicializa o Switcher com LoopMode padrão Infinite
UFrontendCommonAnimatedSwitcher::UFrontendCommonAnimatedSwitcher(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
	, LoopMode(ECarouselLoopMode::Infinite)
{
	
}

void UFrontendCommonAnimatedSwitcher::SetLoopMode(const ECarouselLoopMode NewLoopMode)
{
	// Atualiza o modo de loop
	LoopMode = NewLoopMode;
}

void UFrontendCommonAnimatedSwitcher::PreviousItem(bool& bDidMove)
{
	// Assume que não houve movimento até confirmar
	bDidMove = false;
	
	// Ignora se há apenas 1 slot ou se uma transição já está tocando
	if (Slots.Num() <= 1 || IsTransitionPlaying()) return;
	
	// Marca direção pendente como "anterior"
	bPendingPrev = true;
	
	// Cancela qualquer pendente de "próximo"
	bPendingNext = false; 
	
	// PreviousItem já iniciou a transição — Pending contém o índice do destino
	int32 CurIndex = GetPendingActiveWidgetIndex();
	
	// Fallback para o índice ativo caso a transição já tenha completado antes desta leitura
	if (CurIndex == INDEX_NONE) CurIndex = GetActiveWidgetIndex();
	
	// Calcula o índice anterior
	int32 PrevIndex = CurIndex - 1;

	switch (LoopMode)
	{
	case ECarouselLoopMode::Stop:
		// Bloqueia na borda esquerda — não move
		if (PrevIndex < 0)
		{
			// Sem movimento, limpa o pending
			bDidMove = false; 
			bPendingPrev = false; 
			
			return;
		}
		
		// Move para o slot anterior
		SetActiveWidgetIndex(PrevIndex);
		bDidMove = true;

		break;
	
	case ECarouselLoopMode::Wrap:
		// Salta para o último slot ao ultrapassar a borda esquerda
		PrevIndex = PrevIndex < 0 ? Slots.Num() - 1 : PrevIndex;

		// Move para o slot calculado
		SetActiveWidgetIndex(PrevIndex);
		bDidMove = true;

		break;
		
	case ECarouselLoopMode::Infinite:
		// Move livremente para a esquerda — reposicionamento silencioso ocorre em HandleSlateIsTransitioningChanged
		SetActiveWidgetIndex(PrevIndex);
		bDidMove = true;

		break;
	}
}

void UFrontendCommonAnimatedSwitcher::NextItem(bool& bDidMove)
{
	// Assume que não houve movimento até confirmar
	bDidMove = false;
	
	// Ignora se há apenas 1 slot ou se uma transição já está tocando
	if (Slots.Num() <= 1 || IsTransitionPlaying()) return;
	
	// Cancela qualquer pendente de "anterior"
	bPendingPrev = false;
	
	// Marca direção pendente como "proxima"
	bPendingNext = true;
	
	// NextItem já iniciou a transição — Pending contém o índice do destino
	int32 CurIndex = GetPendingActiveWidgetIndex();
	
	// Fallback para o índice ativo caso a transição já tenha completado antes desta leitura
	if (CurIndex == INDEX_NONE) CurIndex = GetActiveWidgetIndex();
	
	// Calcula o proximo índice
	int32 NextIndex = CurIndex + 1;
	
	switch (LoopMode)
	{
	case ECarouselLoopMode::Stop:
		// Bloqueia na borda direita — não move
		if (NextIndex >  Slots.Num() - 1)
		{
			// Sem movimento, limpa o pending
			bDidMove = false; 
			bPendingNext = false; return;
		}

		// Move para o proximo slot
		SetActiveWidgetIndex(NextIndex);
		bDidMove = true;

		break;
		
	case ECarouselLoopMode::Wrap:
		// Salta para o primeiro slot ao ultrapassar a borda direita
		NextIndex = NextIndex > Slots.Num() - 1 ? 0 : NextIndex;
		
		// Move para o slot calculado
		SetActiveWidgetIndex(NextIndex);
		bDidMove = true;

		break;
		
	case ECarouselLoopMode::Infinite:
		// Move livremente para a direita — reposicionamento silencioso ocorre em HandleSlateIsTransitioningChanged
		SetActiveWidgetIndex(NextIndex);
		bDidMove = true;

		break;
	}
	
}

void UFrontendCommonAnimatedSwitcher::ClearPendingNavigation()
{
	bPendingNext = false;
	bPendingPrev = false;
}

void UFrontendCommonAnimatedSwitcher::HandleSlateIsTransitioningChanged(bool bIsTransitioning)
{
	// Só processa ao fim da transição e apenas no modo Infinite
	if (bIsTransitioning || LoopMode != ECarouselLoopMode::Infinite) return;
	
	Super::HandleSlateIsTransitioningChanged(bIsTransitioning);
	
	const int32 ActiveIndex = GetActiveWidgetIndex();
	
	// Ao chegar no slot-fantasma direito, reposiciona silenciosamente para o slot real (Sem Animação)
	if (bPendingNext && ActiveIndex == Slots.Num() - 1)
	{
		bPendingNext = false;
		
		SetDisableTransitionAnimation(true);
		SetActiveWidgetIndex(1);
		SetDisableTransitionAnimation(false);
	}
	// Ao chegar no slot-fantasma esquerdo, reposiciona silenciosamente para o slot real (Sem Animação)
	else if (bPendingPrev && ActiveIndex == 0)
	{
		bPendingPrev = false;
		
		SetDisableTransitionAnimation(true);
		SetActiveWidgetIndex(Slots.Num() - 2);
		SetDisableTransitionAnimation(false);
	}
}

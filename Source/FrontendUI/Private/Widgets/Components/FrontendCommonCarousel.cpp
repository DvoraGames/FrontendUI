// DvoraGames All Rights Reserved

#include "Widgets/Components/FrontendCommonCarousel.h"

#include "CommonTextBlock.h"
#include "Blueprint/WidgetTree.h"
#include "Widgets/Components/FrontendCommonAnimatedSwitcher.h"
#include "Widgets/Components/Widget_CarouselOption.h"
#include "FrontendTypes/FrontendEnumTypes.h"

UFrontendCommonCarousel::UFrontendCommonCarousel(const FObjectInitializer& ObjectInitializer)
{
	// Permite que o widget receba foco para input de navegação
	SetIsFocusable(true);
}

bool UFrontendCommonCarousel::Initialize()
{
	if (Super::Initialize())
	{
		// Vincula o handler de navegação ao delegate de input direcional
		OnNavigation.BindUObject(this, &UFrontendCommonCarousel::HandleNavigation);
		
		return true;
	}
	
	return false;
}

void UFrontendCommonCarousel::PopulateCarouselLabels(const TArray<FText>& Labels)
{
	// Limpa qualquer navegação pendente
	AnimatedSwitcher_MyCarousel->ClearPendingNavigation();

	// Remove todos os slots existentes
	AnimatedSwitcher_MyCarousel->ClearChildren();
	
	// Guarda os textos do Array do parametro em um novo Array
	OptionsLabels = Labels;
	
	// Verica se o Modo do Carrousel é Infinite
	if (AnimatedSwitcher_MyCarousel->GetLoopMode() == ECarouselLoopMode::Infinite)
	{
		// Verifica se contem menos de 3 opções disponiveis
		if (OptionsLabels.Num() < 3)
		{
			// Define que não é infinite
			bIsInfinite = false;
			
			// Força o modo de Loop do carousel para Wrap
			AnimatedSwitcher_MyCarousel->SetLoopMode(ECarouselLoopMode::Wrap);
		}
		else
		{
			// Define que é infinite
			bIsInfinite = true;
		}
	}
	
	if (bIsInfinite)
	{
		// Modo Infinite: adiciona slot fantasma no início (cópia do último) para ilusão de loop
		AnimatedSwitcher_MyCarousel->AddChild(CreateCarouselOption(OptionsLabels.Last()));
	}
	
	// Adiciona os slots reais das opções
	for (const FText& Label : OptionsLabels)
	{
		AnimatedSwitcher_MyCarousel->AddChild(CreateCarouselOption(Label));
	}
	
	if (bIsInfinite)
	{
		// Modo Infinite: adiciona slot fantasma no final (cópia do primeiro) para ilusão de loop
		AnimatedSwitcher_MyCarousel->AddChild(CreateCarouselOption(OptionsLabels[0]));
	}
	
	// Reseta o índice selecionado para o primeiro elemento
	SelectedIndex = 0;
	
	// Atualiza o Switcher para exibir o primeiro slot sem animação
	SetSelectedByIndex(0);
	
	// Notifica o Blueprint que a população foi concluída
	BP_OnOptionsPopulated(OptionsLabels.Num());
}

void UFrontendCommonCarousel::SetSelectedByIndex(int32 InIndexValue)
{
	// Aborta com um log se o Array OptionsLabels não tiver um indice valido
	if (!OptionsLabels.IsValidIndex(InIndexValue))
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to set CommonCarousel to an out of bounds index: %i"), InIndexValue);
		return;
	}
	
	// Modo Infinite: desloca +1 para compensar o slot fantasma no início
	const int32 SwitcherIndex = bIsInfinite ? InIndexValue + 1 : InIndexValue;
	
	// Limpa qualquer animação pendente
	AnimatedSwitcher_MyCarousel->ClearPendingNavigation();
	
	// Desabilita a Animação do Carousel
	AnimatedSwitcher_MyCarousel->SetDisableTransitionAnimation(true);
	
	// Seleciona o slot com base no Indice passado
	AnimatedSwitcher_MyCarousel->SetActiveWidgetIndex(SwitcherIndex);
	
	// Habilita a Animação do Carousel
	AnimatedSwitcher_MyCarousel->SetDisableTransitionAnimation(false);
	
	// Define que o indice selecionado é o do parametro (sem offset)
	SelectedIndex = InIndexValue;
	
	// Notifica o Blueprint a opção selecionada
	BP_OnOptionSelected(SelectedIndex);
}

int32 UFrontendCommonCarousel::GetSelectedIndex() const
{
	// Retorna o indice selecionado
	return SelectedIndex;
}

void UFrontendCommonCarousel::SetSelectedByText(const FText InTextValue)
{
	// Busca e armazena o índice da primeira opção cujo texto seja igual ao procurado
	const int32 FoundIndex = OptionsLabels.IndexOfByPredicate(
		[InTextValue](const FText& TextItem)->bool
		{
			// Retorna true se o texto do item for igual ao procurado
			return TextItem.EqualTo(InTextValue);
		}
		);
	
	// Se encontrou:
	if (FoundIndex != INDEX_NONE)
	{
		// Atualiza o índice interno e dispara os eventos do rotator
		SetSelectedByIndex(FoundIndex);
	}
}

FText UFrontendCommonCarousel::GetSelectedText() const
{
	if (OptionsLabels.IsValidIndex(SelectedIndex))
	{
		// Retorna o texto da opção correspondente ao índice selecionado
		return OptionsLabels[SelectedIndex];
	}
	
	// Retorna o texto nulo caso Carousel ainda não populado ou índice inválido
	return FText();
}

FNavigationReply UFrontendCommonCarousel::NativeOnNavigation(const FGeometry& MyGeometry,
	const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply)
{	
	
	// Verifica a direção do input de navegação recebido
	switch (InNavigationEvent.GetNavigationType())
	{
	case EUINavigation::Left:
	case EUINavigation::Right:
		// Redireciona Left/Right para o handler customizado de navegação
		return FNavigationReply::Custom(OnNavigation);
	default:
		// Demais direções usam o comportamento padrão do Unreal
		return InDefaultReply;
	}
}

TSharedPtr<SWidget> UFrontendCommonCarousel::HandleNavigation(const EUINavigation UINavigation)
{
    // Verifica se o input é para a esquerda e aciona o shift com flag de navegação
	if (UINavigation == EUINavigation::Left)
	{
		ShiftLeftInternal(true);
	}
	// Verifica se o input é para a direita e aciona o shift com flag de navegação
	else if (UINavigation == EUINavigation::Right)
	{
		ShiftRightInternal(true);
	}
	
	// Retorna nullptr para manter o foco neste widget
	return nullptr;
}

void UFrontendCommonCarousel::ShiftLeft()
{
	// Encaminha para a implementação interna do movimento para a esquerda executado por clique
	ShiftLeftInternal(false);
}

void UFrontendCommonCarousel::ShiftRight()
{
	// Encaminha para a implementação interna do movimento para a direita executado por clique
	ShiftRightInternal(false);
}

TArray<UCommonTextBlock*> UFrontendCommonCarousel::GetCarouselTextBlocks() const
{
	// Array local para guardar provisoriamente os TextBlocks
	TArray<UCommonTextBlock*> TextBlocks;
	
	// Percorre todos os filhos do Switcher
	for (UWidget* CarouselChild : AnimatedSwitcher_MyCarousel->GetAllChildren())
	{
		// Tenta Converter o Slot em UWidget_CarouselOption
		if (const UWidget_CarouselOption* Option = Cast<UWidget_CarouselOption>(CarouselChild))
		{
			// Pega o TextBlock usando a função helper da classe e armazena no Array
			TextBlocks.Add(Option->GetTextBlock());
		}
	}
	
	// Retorna o Array com os Text Blocks
	return TextBlocks;
}

void UFrontendCommonCarousel::ShiftLeftInternal(bool bFromNavigation)
{
	// Aborta se o widget estiver desabilitado para interação
	if (!IsInteractionEnabled()) return;
	
    // bDidMove é preenchido por PreviousItem — true se houve movimento (não estava no limite)
	bool bDidMove = false;
	
    // Inicia a transição para o item anterior e preenche bDidMove com o resultado
	AnimatedSwitcher_MyCarousel->PreviousItem(bDidMove);
	
	// Só atualiza o estado se o Switcher confirmou que houve movimento
	if (bDidMove)
	{
        // PreviousItem já iniciou a transição — Pending contém o índice do destino
		int32 SwitcherIndex = AnimatedSwitcher_MyCarousel->GetPendingActiveWidgetIndex();
		
        // Fallback para o índice ativo caso a transição já tenha completado antes desta leitura
		if (SwitcherIndex == INDEX_NONE) 
			SwitcherIndex = AnimatedSwitcher_MyCarousel->GetActiveWidgetIndex();
		
        // Desconta o slot fantasma do início (+1 no populate) para obter o índice real da opção
		SelectedIndex = bIsInfinite ? SwitcherIndex - 1 : SwitcherIndex;
		
        // Dispara o delegate Blueprint apenas se houver listeners vinculados
		if (OnCarouselRotatedWithDirection.IsBound())
		{
			// Dispara o delegate de rotação do Carousel informando o index e a direção
			OnCarouselRotatedWithDirection.Broadcast(SelectedIndex, ECarouselDirection::Left);
		}
		
		// Dispara o delegate nativo com o índice e a flag de origem
		OnRotatedEvent.Broadcast(SelectedIndex, bFromNavigation);
	}
}

void UFrontendCommonCarousel::ShiftRightInternal(bool bFromNavigation)
{
	// Aborta se o widget estiver desabilitado para interação
	if (!IsInteractionEnabled()) return;
	
	// bDidMove é preenchido por NextItem — true se houve movimento (não estava no limite)
	bool bDidMove = false;
	
	// Inicia a transição para o proximo item e preenche bDidMove com o resultado
	AnimatedSwitcher_MyCarousel->NextItem(bDidMove);
		
	// Só atualiza o estado se o Switcher confirmou que houve movimento
	if (bDidMove)
	{
		// NextItem já iniciou a transição — Pending contém o índice do destino
		int32 SwitcherIndex = AnimatedSwitcher_MyCarousel->GetPendingActiveWidgetIndex();
		
		// Fallback para o índice ativo caso a transição já tenha completado antes desta leitura
		if (SwitcherIndex == INDEX_NONE)
			SwitcherIndex = AnimatedSwitcher_MyCarousel->GetActiveWidgetIndex();
		
		// Desconta o slot fantasma do início (+1 no populate) para obter o índice real da opção
		SelectedIndex = bIsInfinite ? SwitcherIndex - 1 : SwitcherIndex;
		
		// Dispara o delegate Blueprint apenas se houver listeners vinculados
		if (OnCarouselRotatedWithDirection.IsBound())
		{
			// Dispara o delegate de rotação do Carousel informando o index e a direção
			OnCarouselRotatedWithDirection.Broadcast(SelectedIndex, ECarouselDirection::Right);
		}
			
		// Dispara o delegate nativo com o índice e a flag de origem
		OnRotatedEvent.Broadcast(SelectedIndex, bFromNavigation);
	}
}

UWidget_CarouselOption* UFrontendCommonCarousel::CreateCarouselOption(const FText& InText) const
{
	// Instancia o widget de opção usando a classe configurada no Editor
	UWidget_CarouselOption* OptionWiget = CreateWidget<UWidget_CarouselOption>(GetOwningPlayer(), OptionWidgetClass);
		
	// Define o texto exibido neste slot
	OptionWiget->SetOptionText(InText);
		
	// Retorna o widget para ser adicionado ao switcher
	return OptionWiget;
}

// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "FrontendCommonCarousel.generated.h"

class UFrontendCommonAnimatedSwitcher;
class UWidget_CarouselOption;
class UCommonTextBlock;
class UCommonAnimatedSwitcher;

// Direção da rotação do Carousel
UENUM(BlueprintType)
enum class ECarouselDirection: uint8
{
	Right,
	Left,
};

// Delegate disparado ao rotacionar o Carousel, informando o índice e a direção
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCarouselRotatedWithDirection, int32, IndexValue, ECarouselDirection, CarouselDir);

/**
 * UFrontendCommonCarousel
 *
 * Widget de Carousel animado — navega entre opções via Common Animated Switcher
 * com suporte a loop infinito e input de navegação por gamepad/teclado.
 */
UCLASS(meta = (DisableNativeTick))
class FRONTENDUI_API UFrontendCommonCarousel : public UCommonButtonBase
{
	GENERATED_BODY()
	
public:
	explicit UFrontendCommonCarousel(const FObjectInitializer& ObjectInitializer);
	
	//~ Begin UCommonButtonBase Interface
	virtual bool Initialize() override;
	//~ End UCommonButtonBase Interface
	
	// ----------------------------------------------------------
	// Navigation
	// ----------------------------------------------------------

	// Intercepta input de navegação do gamepad/teclado para acionar ShiftLeft/Right
	virtual FNavigationReply NativeOnNavigation(const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply) override;
	
	// Delegate para customizar o comportamento de navegação externamente
	FNavigationDelegate OnNavigation;
	
	// Processa o input de navegação e retorna o widget de destino
	TSharedPtr<SWidget> HandleNavigation(EUINavigation UINavigation);
	
	// ----------------------------------------------------------
	// Behavior
	// ----------------------------------------------------------
	
	// Cria os slots do Switcher com base nos textos fornecidos
	UFUNCTION(BlueprintCallable, Category="Behavior")
	void PopulateCarouselLabels (const TArray<FText>& Labels);
	
	// Seleciona a opção pelo índice
	UFUNCTION(BlueprintCallable, Category="Behavior")
	void SetSelectedByIndex(int32 InIndexValue);
	
	// Retorna o índice da opção atualmente selecionada
	UFUNCTION(BlueprintCallable, Category="Behavior")
	int32 GetSelectedIndex() const;
	
	// Seleciona a opção pelo texto
	UFUNCTION(BlueprintCallable, Category="Behavior")
	void SetSelectedByText(FText InTextValue);
	
	// Retorna o texto da opção atualmente selecionada
	UFUNCTION(BlueprintCallable, Category="Behavior")
	FText GetSelectedText() const;
	
	// Navega para a opção anterior
	UFUNCTION(BlueprintCallable, Category="Behavior")
	void ShiftLeft();
	
	// Navega para a próxima opção
	UFUNCTION(BlueprintCallable, Category="Behavior")
	void ShiftRight();
	
	// Retorna todos os TextBlocks dos slots do Carousel (Usado para aplicar estilo via BP)
	UFUNCTION(BlueprintPure, Category="Behavior")
	TArray<UCommonTextBlock*> GetCarouselTextBlocks() const;
	
	// ----------------------------------------------------------
	// Delegates
	// ----------------------------------------------------------
	
	// Delegate Blueprint — disparado ao rotacionar com índice e direção
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnCarouselRotatedWithDirection OnCarouselRotatedWithDirection;
	
	// Evento(Delegate) nativo — disparado ao rotacionar com índice e flag de navegação por input
	DECLARE_EVENT_TwoParams(UFrontendCommonCarousel, FCarouselRotated, int32, bool)
	FCarouselRotated OnRotatedEvent;
	
protected:
	// ----------------------------------------------------------
	// Handlers
	// ----------------------------------------------------------
	
	// Executa a rotação para a esquerda e dispara os delegates
	void ShiftLeftInternal(bool bFromNavigation);
	
	// Executa a rotação para a direita e dispara os delegates
	void ShiftRightInternal(bool bFromNavigation);
	
	// ----------------------------------------------------------
	// Blueprint Events
	// ----------------------------------------------------------
	
	// Evento Blueprint disparado após popular os slots
	UFUNCTION(BlueprintImplementableEvent, Category = Events, meta = (DisplayName = "On Options Populated"))
	void BP_OnOptionsPopulated(int32 Count);
	
	// Evento Blueprint disparado ao selecionar uma opção
	UFUNCTION(BlueprintImplementableEvent, Category = Events, meta = (DisplayName = "On Options Selected"))
	void BP_OnOptionSelected(int32 Index);
	
	// ----------------------------------------------------------
	// Bound Widgets
	// ----------------------------------------------------------
	
	// Common Animated Switcher customizado que exibe os slots do Carousel
	UPROPERTY(BlueprintReadOnly, Category = CommonCarousel, meta=(BindWidget))
	TObjectPtr<UFrontendCommonAnimatedSwitcher> AnimatedSwitcher_MyCarousel;
	
	// ----------------------------------------------------------
	// Runtime Data
	// ----------------------------------------------------------
	
	// Array de textos das opções populadas
	TArray<FText> OptionsLabels;
	
	// Classe do widget de opção instanciada para cada slot
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWidget_CarouselOption> OptionWidgetClass;
	
	// Índice da opção atualmente selecionada
	int32 SelectedIndex = 0;
	
	// True se o Carousel opera em modo Infinite (loop com slots fantasmas nas bordas)
	bool bIsInfinite;
	
	// Função Helper que instancia e configura um slot de opção com o texto fornecido
	UWidget_CarouselOption* CreateCarouselOption(const FText& InText) const;
};

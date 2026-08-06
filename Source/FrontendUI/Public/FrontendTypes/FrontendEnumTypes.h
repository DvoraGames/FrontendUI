#pragma once

// Enum que define o tipo de modal de confirmação a ser exibido
UENUM(BlueprintType)
enum class EConfirmScreenType : uint8
{
	Ok,						// Modal com apenas o botão OK
	YesNo,					// Modal com o botão Yes e NO
	OkCancel,				// Modal com o botão Ok e Cancel
	Unknown UMETA(Hidden)	// Desconhecido — não aparece no nó do BP
};

// Enum que define o resultado do botão clicado no modal de confirmação
UENUM(BlueprintType)
enum class EConfirmScreenButtonType : uint8
{
	Confirmed,				// Botão de confirmação clicado (Yes / Ok)
	Cancelled,				// Botão de cancelamento clicado (No / Cancel)
	Closed,					// Botão de fechar clicado (Ok em modais informativos)
	Unknown UMETA(Hidden)	// Desconhecido — não aparece no nó do BP
};

// Enum que indica o motivo pelo qual uma opção foi modificada
UENUM(BlueprintType)
enum class EOptionsListDataModifyReason : uint8
{
	DirectlyModified,		// O usuário alterou a opção diretamente (ex: clicou no carrossel)
	DependencyModified,		// A opção mudou devido a outra opção (ex: preset de Qualidade alterou as Sombras)
	ResetToDefault			// A opção foi restaurada para o valor padrão de fábrica
};

// Enum que define o comportamento do Rotator ao atingir o início/fim da lista
UENUM(BlueprintType)
enum class ERotatorLoopMode : uint8
{
	Stop        UMETA(DisplayName = "Stop at Bounds"),		// Para no limite, sem voltar/avançar
	Infinite    UMETA(DisplayName = "Infinite")				// Volta ao início/fim, girando infinitamente
};

// Enum que define o comportamento do Carousel ao atingir o início/fim da lista
UENUM(BlueprintType)
enum class ECarouselLoopMode : uint8
{
	Stop        UMETA(DisplayName = "Stop at Bounds"),		// Para no limite, sem voltar/avançar
	Wrap        UMETA(DisplayName = "Wrap"),				// Pula direto para o outro extremo
	Infinite    UMETA(DisplayName = "Infinite")				// Rola infinitamente entre as opções
};

// Enum que define como os filhos desta entry se comportam dentro da lista/TreeView.
UENUM(BlueprintType)
enum class EEntryChildrenMode : uint8
{
	None,               // Não possui filhos
	ExpandableManual,   // Usuário clica na row/botao para expandir/colapsar
	ExpandableAuto,     // Expansão é decidida pela própria entry (ex: valor do Carousel)
	AlwaysVisible       // Filhos sempre visíveis, apenas ativados/desativados
};

// Enum que define o tipo de ação de toggle exibida em um botão
UENUM(BlueprintType)
enum class EToggleActionType : uint8
{
	None,		// Nenhuma ação configurada
	Expand,		// Expande/recolhe filhos
	Mute,		// Muta/desmuta um valor
	Link,		// Ativa/desativa um link
};
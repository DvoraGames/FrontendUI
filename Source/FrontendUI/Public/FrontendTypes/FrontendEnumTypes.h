#pragma once

// Enum contendo as opções do tipo do Modal
UENUM(BlueprintType)
enum class EConfirmScreenType : uint8
{
	Ok,						// Modal com apenas o botão OK
	YesNo,					// Modal com o botão Yes e NO
	OkCancel,				// Modal com o botão Ok e Cancel
	Unknown UMETA(Hidden)	// Modal desconhecido e que não aparece no nó da BP
};

// Enum contendo as ações de retorno dos Botões do Modal
UENUM(BlueprintType)
enum class EConfirmScreenButtonType : uint8
{
	Confirmed,				// Confirmado
	Cancelled,				// Cancelado
	Closed,					// Cancelado
	Unknown UMETA(Hidden)	// Desconhecido (não aparece na BP)
};

// Enum que indica o motivo de uma opção ter sido modificada
UENUM(BlueprintType)
enum class EOptionsListDataModifyReason : uint8
{
	DirectlyModified,		// O usuário alterou a opção diretamente (ex: clicou no carrossel)
	DependencyModified,		// A opção mudou devido a outra opção (ex: preset de Qualidade alterou as Sombras)
	ResetToDefault			// A opção foi restaurada para o valor padrão de fábrica
};

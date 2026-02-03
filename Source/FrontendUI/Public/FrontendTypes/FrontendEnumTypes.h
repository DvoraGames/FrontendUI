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

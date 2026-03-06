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

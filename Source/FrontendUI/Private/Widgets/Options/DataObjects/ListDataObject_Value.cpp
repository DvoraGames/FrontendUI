// DvoraGames All Rights Reserved


#include "Widgets/Options/DataObjects/ListDataObject_Value.h"

#include "FrontendFunctionLibrary.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"

void UListDataObject_Value::SetDataDynamicGetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicGetter)
{
	// Armazena o Getter para que subclasses consultem o backend ao inicializar
	DataDynamicGetter = InDynamicGetter;
}

void UListDataObject_Value::SetDataDynamicSetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicSetter)
{
	// Armazena o Setter para que subclasses enviem novos valores ao backend
	DataDynamicSetter = InDynamicSetter;
}

void UListDataObject_Value::ExecuteToggleAction()
{
	// Decide qual ação executar com base no tipo de toggle configurado.
	switch (GetToggleActionType())
	{
		case EToggleActionType::Mute:
			OnExecuteMute();
			break;

		case EToggleActionType::Link:
			OnExcuteLink();
			break;

		default:
			break;
	}
}

void UListDataObject_Value::NotifyListDataModified(UListDataObject_Base* ModifiedData,
	EOptionsListDataModifyReason ModifyReason)
{
	// Atualiza o estado do toggle antes de notificar os listeners.
	SyncToggleActionState();

	Super::NotifyListDataModified(ModifiedData, ModifyReason);
}

void UListDataObject_Value::SyncToggleActionState()
{
	// Verifica qual tipo de ação de toggle está configurado.
	switch (GetToggleActionType())
	{
		case EToggleActionType::Mute:
		{
			// Aborta se não houver Getter configurado.
			if (!DataDynamicGetter ) return;
			
			// Lê e converte o valor atual do backend para float.
			const float CurrentValue = StringToFloat(DataDynamicGetter->GetValueAsString());
			
			// Considera "ativo" quando o valor atual é zero ou negativo (mutado).
			SetbIsToggleActionActive(CurrentValue <= 0);
			break;
		}

		case EToggleActionType::Link:
			break;

		default:
			break;
	}
}

void UListDataObject_Value::OnExecuteMute()
{
	// Aborta se Getter ou Setter não estiverem configurados.
	if (!DataDynamicGetter || !DataDynamicSetter) return;

	// Lê o valor atual do backend e converte para float.
	const float CurrentValue = StringToFloat(DataDynamicGetter->GetValueAsString());

	// Verifica se o som está ativo (valor positivo).
	if (CurrentValue > 0)
	{
		// Salva o valor atual antes de mutar (zerar).
		PreviousValue = LexToString(CurrentValue);
		
		// Envia 0 ao backend, mutando o som.
		DataDynamicSetter->SetValueFromString(TEXT("0"));
	}
	else
	{
		// Restaura o valor salvo para desmutar.
		DataDynamicSetter->SetValueFromString(PreviousValue);
	}

	// Notifica os widgets vinculados sobre a mudança.
	NotifyListDataModified(this);
}

void UListDataObject_Value::OnExcuteLink() {}

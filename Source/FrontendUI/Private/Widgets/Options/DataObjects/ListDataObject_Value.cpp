// DvoraGames All Rights Reserved


#include "Widgets/Options/DataObjects/ListDataObject_Value.h"

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

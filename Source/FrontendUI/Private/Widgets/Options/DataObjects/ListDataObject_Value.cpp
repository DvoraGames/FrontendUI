// DvoraGames All Rights Reserved


#include "Widgets/Options/DataObjects/ListDataObject_Value.h"

void UListDataObject_Value::SetDataDynamicGetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicGetter)
{
	// Salva o getter para que os filhos (ex: Carousel) possam consultar o backend quando inicializarem
	DataDynamicGetter = InDynamicGetter;
}

void UListDataObject_Value::SetDataDynamicSetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicSetter)
{
	// Salva o setter para que os filhos (ex: Carousel) possam enviar os novos dados para o backend
	DataDynamicSetter = InDynamicSetter;
}

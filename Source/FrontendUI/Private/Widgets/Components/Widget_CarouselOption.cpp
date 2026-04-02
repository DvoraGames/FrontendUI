// DvoraGames All Rights Reserved


#include "Widgets/Components/Widget_CarouselOption.h"

#include "CommonTextBlock.h"

void UWidget_CarouselOption::SetOptionText(const FText& Text) const
{
	CommonText_OptionText->SetText(Text);
}

// DvoraGames All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_CarouselOption.generated.h"

class UCommonTextBlock;

UCLASS(Abstract, BlueprintType, meta=(DisableNaiveTick))
class FRONTENDUI_API UWidget_CarouselOption : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetOptionText(const FText& Text) const;
	
	UCommonTextBlock* GetTextBlock() const { return CommonText_OptionText; }

private:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	UCommonTextBlock* CommonText_OptionText;
};

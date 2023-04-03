// Fill out your copyright notice in the Description page of Project Settings.

#include "MercenaryWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "SquadGameInstance.h"

void UMercenaryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
	IncludeFirstMercenary = Cast<UButton>(GetWidgetFromName("INCLUDEFIRSTMERCENARY"));
	if (IncludeFirstMercenary != nullptr)
	{
		IncludeFirstMercenary->OnClicked.AddDynamic(this, &UMercenaryWidget::ClickInCludeFirstMercenary);
	}
	IncludeSecondMercenary = Cast<UButton>(GetWidgetFromName("INCLUDESECONDMERCENARY"));
	if (IncludeSecondMercenary != nullptr)
	{
		IncludeSecondMercenary->OnClicked.AddDynamic(this, &UMercenaryWidget::ClickInCludeSecondMercenary);
	}
	IncludeThirdMercenary = Cast<UButton>(GetWidgetFromName("INCLUDETHIRDMERCENARY"));
	if (IncludeThirdMercenary != nullptr)
	{
		IncludeThirdMercenary->OnClicked.AddDynamic(this, &UMercenaryWidget::ClickInCludeThirdMercenary);
	}

	ExcludeFirstMercenary = Cast<UButton>(GetWidgetFromName("EXCLUDEFIRSTMERCENARY"));
	if (ExcludeFirstMercenary != nullptr)
	{
		ExcludeFirstMercenary->OnClicked.AddDynamic(this, &UMercenaryWidget::ClickExCludeFirstMercenary);
	}

	// юс╫ц //

	FirstMercenaryData.Damage = 1;
	FirstMercenaryData.LifePoint = 3.f;
	FirstMercenaryData.ClassNumber = 0;


	SecondMercenaryData.Damage = 1;
	SecondMercenaryData.LifePoint = 3.f;
	SecondMercenaryData.ClassNumber = 1;

	ThirdMercenaryData.Damage = 1;
	ThirdMercenaryData.LifePoint = 3.f;
	ThirdMercenaryData.ClassNumber = 2;

	//

	ImageArray = { Image_0 , Image_1 , Image_2 };
	ImageTextArray = { ImageText_0 , ImageText_1  , ImageText_2 };
}

void UMercenaryWidget::ClickInCludeFirstMercenary()
{
	USquadGameInstance* gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

	if(gameIns->CharSlot.MaxSlotNum > gameIns->GetCharacterDataNum())
	{
		gameIns->IncludeCharacterData(FirstMercenaryData);
		RefreshMark_Include(FText::FromString("a"));
	}	
}

void UMercenaryWidget::ClickInCludeSecondMercenary()
{
	USquadGameInstance* gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

	if (gameIns->CharSlot.MaxSlotNum > gameIns->GetCharacterDataNum())
	{
		gameIns->IncludeCharacterData(SecondMercenaryData);
		RefreshMark_Include(FText::FromString("b"));
	}
}

void UMercenaryWidget::ClickInCludeThirdMercenary()
{
	USquadGameInstance* gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

	if (gameIns->CharSlot.MaxSlotNum > gameIns->GetCharacterDataNum())
	{
		gameIns->IncludeCharacterData(ThirdMercenaryData);
		RefreshMark_Include(FText::FromString("c"));
	}
}

void UMercenaryWidget::ClickExCludeFirstMercenary()
{
	USquadGameInstance* gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

	if (gameIns->CharSlot.CalSlotNum() == 0)
	{
		UE_LOG(LogClass, Log, TEXT(" No More Exclued "));
	}
	else
	{
		UE_LOG(LogClass, Log, TEXT(" Exclude "));
		gameIns->ExcludeCharacterData();
		RefreshMark_Exclude();
	}
	
}

void UMercenaryWidget::RefreshMark_Include(FText ClassName)
{
	USquadGameInstance* gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	ImageArray[gameIns->CharSlot.CalSlotNum() - 1]->SetColorAndOpacity(FLinearColor::Red);
	ImageTextArray[gameIns->CharSlot.CalSlotNum() - 1]->SetText(ClassName);

}

void UMercenaryWidget::RefreshMark_Exclude()
{
	USquadGameInstance* gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	ImageArray[gameIns->CharSlot.CalSlotNum()]->SetColorAndOpacity(FLinearColor::White);
	ImageTextArray[gameIns->CharSlot.CalSlotNum()]->SetText(FText::FromString("-"));

}
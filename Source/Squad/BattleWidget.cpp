// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "SquadGameInstance.h"
#include "PlayerSquadCharacter.h"
#include "SquadController.h"
#include "SquadCharacterStatComponent.h"
#include "SquadCharacterSkillComponent.h"
#include "SquadCharacter.h"
#include "Components/HorizontalBox.h"
#include "Components/CanvasPanel.h"
#include "Engine/Texture2D.h"
#include "Engine/Engine.h"

void UBattleWidget::NativeConstruct()
{
	Super::NativeConstruct();


	/*
	MoveButton = Cast<UButton>(GetWidgetFromName("MOVEBUTTON"));
	if (MoveButton != nullptr)
	{
		MoveButton->OnClicked.AddDynamic(this, &UBattleWidget::OnClickMoveButton);
	}
	*/
	AttackButton = Cast<UButton>(GetWidgetFromName("ATTACKBUTTON"));
	if (AttackButton != nullptr)
	{
		AttackButton->OnClicked.AddDynamic(this, &UBattleWidget::OnClickAttackButton);
	}
	CoverButton = Cast<UButton>(GetWidgetFromName("COVERBUTTON"));
	if (CoverButton != nullptr)
	{
		CoverButton->OnClicked.AddDynamic(this, &UBattleWidget::OnClickCoverButton);
	}
	ReloadButton = Cast<UButton>(GetWidgetFromName("RELOADBUTTON"));
	if (ReloadButton != nullptr)
	{
		ReloadButton->OnClicked.AddDynamic(this, &UBattleWidget::OnClickReloadButton);
	}
	Skill1Button = Cast<UButton>(GetWidgetFromName("SKILL1BUTTON"));
	if (Skill1Button != nullptr)
	{
		Skill1Button->OnClicked.AddDynamic(this, &UBattleWidget::OnClickSkill1Button);
	}
	Skill2Button = Cast<UButton>(GetWidgetFromName("SKILL2BUTTON"));
	if (Skill2Button != nullptr)
	{
		Skill2Button->OnClicked.AddDynamic(this, &UBattleWidget::OnClickSkill2Button);
	}


	TargetNameBlock = Cast<UTextBlock>(GetWidgetFromName("TARGETNAMEBLOCK"));
	if (TargetNameBlock != nullptr)
	{
		TargetNameBlock->SetText(FText::FromString(InitName));
	}
	CurrentCharacter = Cast<UTextBlock>(GetWidgetFromName("CURRENTCHARACTER"));
	if (CurrentCharacter != nullptr)
	{
		CurrentCharacter->SetText(FText::FromString(InitName));
	}
	WhosTurn = Cast<UTextBlock>(GetWidgetFromName("WHOSTURN"));
	if (WhosTurn != nullptr)
	{
		WhosTurn->SetText(FText::FromString(InitName));
	}

	SelectedCharacterMaxAmmo = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERMAXAMMO"));
	SelectedCharacterCurrentAmmo = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERCURRENTAMMO"));
	SelectedCharacterMaxLifePoint = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERMAXLIFEPOINT"));
	SelectedCharacterCurrentLifePoint = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERCURRENTLIFEPOINT"));

	TargetCharacterMaxAmmo = Cast<UTextBlock>(GetWidgetFromName("TARGETCHARACTERMAXAMMO"));
	TargetCharacterCurrentAmmo = Cast<UTextBlock>(GetWidgetFromName("TARGETCHARACTERCURRENTAMMO"));
	TargetCharacterMaxLifePoint = Cast<UTextBlock>(GetWidgetFromName("TARGETCHARACTERMAXLIFEPOINT"));
	TargetCharacterCurrentLifePoint = Cast<UTextBlock>(GetWidgetFromName("TARGETCHARACTERCURRENTLIFEPOINT"));

	TargetCharacterLifePointMarker = Cast<UTextBlock>(GetWidgetFromName("TARGETCHARACTERLIFEPOINTMARKER"));
	TargetCharacterAmmoMarker = Cast<UTextBlock>(GetWidgetFromName("TARGETCHARACTERAMMOMARKER"));
	TargetCharacterHPSlash = Cast<UTextBlock>(GetWidgetFromName("TARGETCHARACTERHPSLASH"));
	TargetCharacterAmmoSlash = Cast<UTextBlock>(GetWidgetFromName("TARGETCHARACTERAMMOSLASH"));
	
	
	TargetCharacterEvasion = Cast<UTextBlock>(GetWidgetFromName("TARGETCHARACTEREVASION"));
	TargetCharacterDefense = Cast<UTextBlock>(GetWidgetFromName("TARGETCHARACTERDEFENSE"));
	TargetCharacterAccurancy = Cast<UTextBlock>(GetWidgetFromName("TARGETCHARACTERACCURANCY"));
	TargetCharacterCritical = Cast<UTextBlock>(GetWidgetFromName("TARGETCHARACTERCRITICAL"));
	TargetCharacterDamage = Cast<UTextBlock>(GetWidgetFromName("TARGETCHARACTERDAMAGE"));
	TargetCharacterHit = Cast<UTextBlock>(GetWidgetFromName("TARGETCHARACTERHIT"));
	TargetCharacterMaxDamage = Cast<UTextBlock>(GetWidgetFromName("TARGETCHARACTERMAXDAMAGE"));

	TargetCharacterEvasionMarker = Cast<UTextBlock>(GetWidgetFromName("TARGETCHARACTEREVASIONMARKER"));
	TargetCharacterDefenseMarker = Cast<UTextBlock>(GetWidgetFromName("TARGETCHARACTERDEFENSEMARKER"));
	TargetCharacterAccurancyMarker = Cast<UTextBlock>(GetWidgetFromName("TARGETCHARACTERACCURANCYMARKER"));
	TargetCharacterCriticalMarker = Cast<UTextBlock>(GetWidgetFromName("TARGETCHARACTERCRITICALMARKER"));
	TargetCharacterDamageMarker = Cast<UTextBlock>(GetWidgetFromName("TARGETCHARACTERDAMAGEMARKER"));
	TargetCharacterHitMarker = Cast<UTextBlock>(GetWidgetFromName("TARGETCHARACTERHITMARKER"));
	TargetCharacterMaxDamageMarker = Cast<UTextBlock>(GetWidgetFromName("TARGETCHARACTERMAXDAMAGEMARKER"));

	TargetCharacterTotalAccurancy = Cast<UTextBlock>(GetWidgetFromName("TARGETCHARACTERTOTALACCURANCY"));
	TargetCharacterTotalAccurancyMarker = Cast<UTextBlock>(GetWidgetFromName("TARGETCHARACTERTOTALACCURANCYMARKER"));
	TargetCharacterTotalCritical = Cast<UTextBlock>(GetWidgetFromName("TARGETCHARACTERTOTALCRITICAL"));
	TargetCharacterTotalCriticalMarker = Cast<UTextBlock>(GetWidgetFromName("TARGETCHARACTERTOTALCRITICALMARKER"));

	SelectedCharacterAccuracy = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERACCURACY"));
	SelectedCharacterCritical = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERCRITICAL"));
	SelectedCharacterDamage = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERDAMAGE"));
	SelectedCharacterHitCount = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERHITCOUNT"));
	SelectedCharacterMaxDamage = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERMAXDAMAGE"));

	SelectedCharacterLifePointMarker = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERLIFEPOINTMARKER"));
	SelectedCharacterAmmoMarker = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERAMMOMARKER"));
	SelectedCharacterHPSlash = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERHPSLASH"));
	SelectedCharacterAmmoSlash = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERAMMOSLASH"));
	SelectedCharacterEvasion = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTEREVASION"));
	SelectedCharacterDefense = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERDEFENSE"));

	SelectedCharacterAccuracyMarker = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERACCURACYMARKER"));
	SelectedCharacterCriticalMarker = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERCRITICALMARKER"));
	SelectedCharacterMaxDamageMarker = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERMAXDAMAGEMARKER"));
	SelectedCharacterDamageMarker = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERDAMAGEMARKER"));
	SelectedCharacterHitCountMarker = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERHITCOUNTMARKER"));
	SelectedCharacterEvasionMarker = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTEREVASIONMARKER"));
	SelectedCharacterDefenseMarker = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERDEFENSEMARKER"));

	SelectedCharacterSkillName = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERSKILLNAME"));
	SelectedCharacterSkillAccurancy = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERSKILLACCURANCY"));
	SelectedCharacterSkillCritical = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERSKILLCRITICAL"));
	SelectedCharacterSkillCooldown = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERSKILLCOOLDOWN"));
	SelectedCharacterSkillExplanation = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERSKILLEXPLANATION"));
	SelectedCharacterSkillAttribute = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERSKILLATTRIBUTE"));
	SelectedCharacterSkillAttributeEffect = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERSKILLATTRIBUTEEFFECT"));

	SelectedCharacterSkillMarker1 = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERSKILLMARKER1"));
	SelectedCharacterSkillMarker2 = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERSKILLMARKER2"));
	SelectedCharacterSkillMarker3 = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERSKILLMARKER3"));
	SelectedCharacterSkillMarker4 = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERSKILLMARKER4"));
	SelectedCharacterSkillMarker5 = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERSKILLMARKER5"));
	
	Skill0ButtonImage = Cast<UImage>(GetWidgetFromName("SKILL0BUTTONIMAGE"));
	Skill1ButtonImage = Cast<UImage>(GetWidgetFromName("SKILL1BUTTONIMAGE"));
	Skill2ButtonImage = Cast<UImage>(GetWidgetFromName("SKILL2BUTTONIMAGE"));
	Skill3ButtonImage = Cast<UImage>(GetWidgetFromName("SKILL3BUTTONIMAGE"));
	Skill4ButtonImage = Cast<UImage>(GetWidgetFromName("SKILL4BUTTONIMAGE"));
	
	SelectedCharacterDMGGridMarker = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERDMGGRIDMARKER"));
	SelectedCharacterACCGridMarker = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERACCGRIDMARKER"));
	SelectedCharacterCritGridMarker = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERCRITGRIDMARKER"));

	SelectedCharacterDamage0GridMarker= Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERDAMAGE0GRIDMARKER"));
	SelectedCharacterDamage1GridMarker= Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERDAMAGE1GRIDMARKER"));
	SelectedCharacterDamage2GridMarker= Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERDAMAGE2GRIDMARKER"));
	SelectedCharacterDamage3GridMarker= Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERDAMAGE3GRIDMARKER"));
	SelectedCharacterDamage4GridMarker= Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERDAMAGE4GRIDMARKER"));
	SelectedCharacterDamage5GridMarker= Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERDAMAGE5GRIDMARKER"));
	SelectedCharacterDamage6GridMarker= Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERDAMAGE6GRIDMARKER"));
	SelectedCharacterDamage7GridMarker= Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERDAMAGE7GRIDMARKER"));
	SelectedCharacterDamage8GridMarker= Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERDAMAGE8GRIDMARKER"));
	SelectedCharacterDamage9GridMarker= Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERDAMAGE9GRIDMARKER"));
	SelectedCharacterDamage10GridMarker= Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERDAMAGE10GRIDMARKER"));

	SelectedCharacterDamage0perGridMarker= Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERDAMAGE0PERGRIDMARKER"));
	SelectedCharacterDamage1perGridMarker= Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERDAMAGE1PERGRIDMARKER"));
	SelectedCharacterDamage2perGridMarker= Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERDAMAGE2PERGRIDMARKER"));
	SelectedCharacterDamage3perGridMarker= Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERDAMAGE3PERGRIDMARKER"));
	SelectedCharacterDamage4perGridMarker= Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERDAMAGE4PERGRIDMARKER"));
	SelectedCharacterDamage5perGridMarker= Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERDAMAGE5PERGRIDMARKER"));
	SelectedCharacterDamage6perGridMarker = Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERDAMAGE6PERGRIDMARKER"));
	SelectedCharacterDamage7perGridMarker= Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERDAMAGE7PERGRIDMARKER"));
	SelectedCharacterDamage8perGridMarker= Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERDAMAGE8PERGRIDMARKER"));
	SelectedCharacterDamage9perGridMarker= Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERDAMAGE9PERGRIDMARKER"));
	SelectedCharacterDamage10perGridMarker= Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERDAMAGE10PERGRIDMARKER"));


	SelectedCharacterCritperGridMarker= Cast<UTextBlock>(GetWidgetFromName("SELECTEDCHARACTERCRITPERGRIDMARKER"));
	
	SelectedCharacterBattleTable = Cast<UImage>(GetWidgetFromName("SELECTEDCHARACTERBATTLETABLE"));


	SelectedCharacterSkillGraphWhiteBackground = Cast<UImage>(GetWidgetFromName("SELECTEDCHARACTERSKILLGRAPHWHITEBACKGROUND"));
	SelectedCharacterSkillGraphBlackBackground = Cast<UImage>(GetWidgetFromName("SELECTEDCHARACTERSKILLGRAPHBLACKBACKGROUND"));
	SelectedCharacterSkillGraphMarker = Cast<UHorizontalBox>(GetWidgetFromName("SELECTEDCHARACTERSKILLGRAPHMARKER"));

	BattlewidgetRightPanelBackground = Cast<UCanvasPanel>(GetWidgetFromName("BATTLEWIDGETRIGHTPANELBACKGROUND"));
	BattlewidgetLeftPanelBackground = Cast<UCanvasPanel>(GetWidgetFromName("BATTLEWIDGETLEFTPANELBACKGROUND"));

	Skill1CooldownMarker = Cast<UBorder>(GetWidgetFromName("SKILL1COOLDOWNMARKER"));
	Skill2CooldownMarker = Cast<UBorder>(GetWidgetFromName("SKILL2COOLDOWNMARKER"));
	Skill1CooldownValue = Cast<UTextBlock>(GetWidgetFromName("SKILL1COOLDOWNVALUE"));
	Skill2CooldownValue = Cast<UTextBlock>(GetWidgetFromName("SKILL2COOLDOWNVALUE"));

	/////////////// 문자열 공백으로 초기화 ///////////////////////////////

	CurrentCharacter->SetText(FText::FromString(TEXT("")));
	SelectedCharacterMaxAmmo->SetText(FText::FromString(TEXT("")));
	SelectedCharacterCurrentAmmo->SetText(FText::FromString(TEXT("")));
	SelectedCharacterMaxLifePoint->SetText(FText::FromString(TEXT("")));
	SelectedCharacterCurrentLifePoint->SetText(FText::FromString(TEXT("")));

	TargetNameBlock->SetText(FText::FromString(TEXT("")));
	TargetCharacterMaxAmmo->SetText(FText::FromString(TEXT("")));
	TargetCharacterCurrentAmmo->SetText(FText::FromString(TEXT("")));
	TargetCharacterMaxLifePoint->SetText(FText::FromString(TEXT("")));
	TargetCharacterCurrentLifePoint->SetText(FText::FromString(TEXT("")));

	TargetCharacterLifePointMarker->SetText(FText::FromString(TEXT("")));
	TargetCharacterAmmoMarker->SetText(FText::FromString(TEXT("")));
	TargetCharacterHPSlash->SetText(FText::FromString(TEXT("")));
	TargetCharacterAmmoSlash->SetText(FText::FromString(TEXT("")));

	TargetCharacterEvasion->SetText(FText::FromString(TEXT("")));
	TargetCharacterDefense->SetText(FText::FromString(TEXT("")));
	TargetCharacterAccurancy->SetText(FText::FromString(TEXT("")));
	TargetCharacterCritical->SetText(FText::FromString(TEXT("")));
	TargetCharacterDamage->SetText(FText::FromString(TEXT("")));
	TargetCharacterHit->SetText(FText::FromString(TEXT("")));
	TargetCharacterMaxDamage->SetText(FText::FromString(TEXT("")));

	TargetCharacterEvasionMarker->SetText(FText::FromString(TEXT("")));
	TargetCharacterDefenseMarker->SetText(FText::FromString(TEXT("")));
	TargetCharacterAccurancyMarker->SetText(FText::FromString(TEXT("")));
	TargetCharacterCriticalMarker->SetText(FText::FromString(TEXT("")));
	TargetCharacterDamageMarker->SetText(FText::FromString(TEXT("")));
	TargetCharacterHitMarker->SetText(FText::FromString(TEXT("")));
	TargetCharacterMaxDamageMarker->SetText(FText::FromString(TEXT("")));

	TargetCharacterTotalAccurancy->SetText(FText::FromString(TEXT("")));
	TargetCharacterTotalAccurancyMarker->SetText(FText::FromString(TEXT("")));
	TargetCharacterTotalCritical->SetText(FText::FromString(TEXT("")));
	TargetCharacterTotalCriticalMarker->SetText(FText::FromString(TEXT("")));

	SelectedCharacterAccuracy->SetText(FText::FromString(TEXT("")));
	SelectedCharacterCritical->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDamage->SetText(FText::FromString(TEXT("")));
	SelectedCharacterHitCount->SetText(FText::FromString(TEXT("")));
	SelectedCharacterMaxDamage->SetText(FText::FromString(TEXT("")));
	SelectedCharacterEvasion->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDefense->SetText(FText::FromString(TEXT("")));

	SelectedCharacterLifePointMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterAmmoMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterHPSlash->SetText(FText::FromString(TEXT("")));
	SelectedCharacterAmmoSlash->SetText(FText::FromString(TEXT("")));

	
	SelectedCharacterAccuracyMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterCriticalMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterMaxDamageMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDamageMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterHitCountMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterEvasionMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDefenseMarker->SetText(FText::FromString(TEXT("")));

	SelectedCharacterSkillName->SetText(FText::FromString(TEXT("")));
	SelectedCharacterSkillAccurancy->SetText(FText::FromString(TEXT("")));
	SelectedCharacterSkillCritical->SetText(FText::FromString(TEXT("")));
	SelectedCharacterSkillCooldown->SetText(FText::FromString(TEXT("")));
	SelectedCharacterSkillExplanation->SetText(FText::FromString(TEXT("")));
	SelectedCharacterSkillAttribute->SetText(FText::FromString(TEXT("")));
	SelectedCharacterSkillAttributeEffect->SetText(FText::FromString(TEXT("")));

	SelectedCharacterSkillMarker1->SetText(FText::FromString(TEXT("")));
	SelectedCharacterSkillMarker2->SetText(FText::FromString(TEXT("")));
	SelectedCharacterSkillMarker3->SetText(FText::FromString(TEXT("")));
	SelectedCharacterSkillMarker4->SetText(FText::FromString(TEXT("")));
	SelectedCharacterSkillMarker5->SetText(FText::FromString(TEXT("")));

	
	SelectedCharacterDMGGridMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterACCGridMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterCritGridMarker->SetText(FText::FromString(TEXT("")));
		
	SelectedCharacterDamage0GridMarker->SetText(FText::FromString(TEXT("")));	
	SelectedCharacterDamage1GridMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDamage2GridMarker->SetText(FText::FromString(TEXT("")));	
	SelectedCharacterDamage3GridMarker->SetText(FText::FromString(TEXT("")));	
	SelectedCharacterDamage4GridMarker->SetText(FText::FromString(TEXT("")));	
	SelectedCharacterDamage5GridMarker->SetText(FText::FromString(TEXT("")));	
	SelectedCharacterDamage6GridMarker->SetText(FText::FromString(TEXT("")));	
	SelectedCharacterDamage7GridMarker->SetText(FText::FromString(TEXT("")));	
	SelectedCharacterDamage8GridMarker->SetText(FText::FromString(TEXT("")));	
	SelectedCharacterDamage9GridMarker->SetText(FText::FromString(TEXT("")));	
	SelectedCharacterDamage10GridMarker->SetText(FText::FromString(TEXT("")));

	SelectedCharacterDamage0perGridMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDamage1perGridMarker->SetText(FText::FromString(TEXT("")));	
	SelectedCharacterDamage2perGridMarker->SetText(FText::FromString(TEXT("")));	
	SelectedCharacterDamage3perGridMarker->SetText(FText::FromString(TEXT("")));	
	SelectedCharacterDamage4perGridMarker->SetText(FText::FromString(TEXT("")));	
	SelectedCharacterDamage5perGridMarker->SetText(FText::FromString(TEXT("")));	
	SelectedCharacterDamage6perGridMarker->SetText(FText::FromString(TEXT("")));	
	SelectedCharacterDamage7perGridMarker->SetText(FText::FromString(TEXT("")));	
	SelectedCharacterDamage8perGridMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDamage9perGridMarker->SetText(FText::FromString(TEXT("")));	
	SelectedCharacterDamage10perGridMarker->SetText(FText::FromString(TEXT("")));

	
	SelectedCharacterCritperGridMarker->SetText(FText::FromString(TEXT("")));

	SelectedCharacterBattleTable->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.0f));
	SelectedCharacterSkillGraphBlackBackground->SetColorAndOpacity(FLinearColor(0.01f, 0.01f, 0.005f, 0.0f));
	SelectedCharacterSkillGraphWhiteBackground->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.0f));
	
	
	Skill1CooldownMarker->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
	Skill2CooldownMarker->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
	Skill1CooldownValue->SetText(FText::FromString(TEXT("")));
	Skill2CooldownValue->SetText(FText::FromString(TEXT("")));

	/////////////////////////////////////////////////////////////////////////

	SelectedCharacterSkillGraphMarker->SetVisibility(ESlateVisibility::Hidden);
	AttackButton->SetVisibility(ESlateVisibility::Hidden);
	Skill1Button->SetVisibility(ESlateVisibility::Hidden);
	ReloadButton->SetVisibility(ESlateVisibility::Hidden);
	CoverButton->SetVisibility(ESlateVisibility::Hidden);
	Skill2Button->SetVisibility(ESlateVisibility::Hidden);



	// Init //

	//Set_BattleWidgetSkilliconOpacity(false);
}


void UBattleWidget::OnClickMoveButton() // 삭제
{
	auto GameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

	auto Controller = GetWorld()->GetFirstPlayerController();
	if (Controller == Cast<ASquadController>(Controller))
	{
		//Cast<ASquadController>(Controller)->SetCharacterAction(ECharAction::MOVE);
		//if (GameIns->SelectedCharacter != nullptr && Cast<APlayerSquadCharacter>(GameIns->SelectedCharacter)->StateEnum != EStateEnum::SE_End)
			//Cast<APlayerSquadCharacter>(GameIns->SelectedCharacter)->StateEnum = EStateEnum::SE_Move;
	}
	
}

void UBattleWidget::OnClickAttackButton()
{
	auto GameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	if (GameIns->SelectedCharacter != nullptr) // && Cast<APlayerSquadCharacter>(GameIns->SelectedCharacter)->CurrentAmmo > 0)
	{
		Cast<APlayerSquadCharacter>(GameIns->SelectedCharacter)->SetShotReady();
	}
}

void UBattleWidget::OnClickCoverButton()
{
	auto GameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

		if (GameIns->SelectedCharacter != nullptr)
		{
			Cast<APlayerSquadCharacter>(GameIns->SelectedCharacter)->SetCoverReady();

		}
	
}

void UBattleWidget::OnClickReloadButton()
{
	auto GameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

		if (GameIns->SelectedCharacter != nullptr)
		{
			Cast<APlayerSquadCharacter>(GameIns->SelectedCharacter)->SetReloadReady();
		}
	
}

void UBattleWidget::OnClickSkill1Button()
{
	auto GameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	if (GameIns->SelectedCharacter != nullptr)
	{
		Cast<APlayerSquadCharacter>(GameIns->SelectedCharacter)->SetSkill1();
	}
}

void UBattleWidget::OnClickSkill2Button()
{
	auto GameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	if (GameIns->SelectedCharacter != nullptr)
	{
		Cast<APlayerSquadCharacter>(GameIns->SelectedCharacter)->SetSkill2();
	
	}
	
}

///////////////////////////////////////////////////////////////////

void UBattleWidget::ChangeTargetName(FString NAME)
{
	TargetNameBlock->SetText(FText::FromString(NAME));
}

void UBattleWidget::ChangeCurrentCharName(FString NAME)
{
	CurrentCharacter->SetText(FText::FromString(NAME));
}

void UBattleWidget::ChangeWhosTurnName(bool NAME)
{
	if (!NAME)
	{
		WhosTurn->SetText(FText::FromString(L"My Turn"));
	}
	else
	{
		WhosTurn->SetText(FText::FromString(L"Enemy Turn"));
	}
}

void UBattleWidget::ChangeCurrentAmmoText(float CurrentAmmo)
{
	FString CAText = FString::FromInt(CurrentAmmo);
	SelectedCharacterCurrentAmmo->SetText(FText::FromString(CAText));
}

void UBattleWidget::ChangeMaxAmmoText(float MaxAmmo)
{
	FString MAText = FString::FromInt(MaxAmmo);
	SelectedCharacterMaxAmmo->SetText(FText::FromString(MAText));
}

void UBattleWidget::ChangeSkillButtonImage(APlayerSquadCharacter* PlayerCharacter)//UTexture2D* InTexture)
{
	if (Skill1ButtonTexture2D != nullptr) {
		Skill1ButtonTexture2D = nullptr;
		Skill2ButtonTexture2D = nullptr;
	}
	
	auto skill1Icon_DataPath = PlayerCharacter->CharacterSkillComp->Get_Skill1Data()->Skill_icon_DataPath;
	auto skill2Icon_DataPath = PlayerCharacter->CharacterSkillComp->Get_Skill2Data()->Skill_icon_DataPath;




	Skill1ButtonTexture2D = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *skill1Icon_DataPath));
	Skill2ButtonTexture2D = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *skill2Icon_DataPath));


	Skill1ButtonImage->SetBrushFromTexture(Skill1ButtonTexture2D);
	Skill2ButtonImage->SetBrushFromTexture(Skill2ButtonTexture2D);

	if(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->IsBattleStart == true) {
		if (PlayerCharacter->Character_Skill1_Cooldown == 0) {
			Skill1Button->BackgroundColor = FLinearColor(1.f, 1.f, 1.f, 1.0f);
			Skill1ButtonImage->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.0f));
		}
		else if (PlayerCharacter->Character_Skill1_Cooldown > 0) {
			Skill1Button->BackgroundColor = FLinearColor(1.f, 1.f, 1.f, 0.5f);
			Skill1ButtonImage->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.5f));
		}

		if (PlayerCharacter->Character_Skill2_Cooldown == 0) {
			Skill2Button->BackgroundColor = FLinearColor(1.f, 1.f, 1.f, 1.0f);
			Skill2ButtonImage->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.0, 1.0f));
		} 
		else if (PlayerCharacter->Character_Skill2_Cooldown > 0) {
			Skill2Button->BackgroundColor = FLinearColor(1.f, 1.f, 1.f, 0.5f);
			Skill2ButtonImage->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.5f));
		}
	}


	UpdateCharacterSkillMarker(PlayerCharacter);
}

void UBattleWidget::UpdateCharacterSkillMarker(APlayerSquadCharacter* PlayerCharacter)
{
	if (PlayerCharacter->Character_Skill1_Cooldown > 0) {
		Skill1CooldownMarker->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.5f));
		Skill1CooldownValue->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerCharacter->Character_Skill1_Cooldown)));
	}
	else if (PlayerCharacter->Character_Skill1_Cooldown == 0) {
		Skill1CooldownMarker->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
		Skill1CooldownValue->SetText(FText::FromString(TEXT("")));
	}

	if (PlayerCharacter->Character_Skill2_Cooldown > 0) {
		Skill2CooldownMarker->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.5f));
		Skill2CooldownValue->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerCharacter->Character_Skill2_Cooldown)));
	}
	else if (PlayerCharacter->Character_Skill2_Cooldown == 0) {
		Skill2CooldownMarker->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
		Skill2CooldownValue->SetText(FText::FromString(TEXT("")));
	}
}


void UBattleWidget::UpdateCharacterInfoText(FText Name, float MaxAmmo, float CurrentAmmo, float MaxLifePoint, float CurrentLifePoint)
{
	
	CurrentCharacter->SetText(Name);
	SelectedCharacterMaxAmmo->SetText(FText::FromString(FString::FromInt(MaxAmmo)));
	SelectedCharacterCurrentAmmo->SetText(FText::FromString(FString::FromInt(CurrentAmmo)));
	SelectedCharacterMaxLifePoint->SetText(FText::FromString(FString::FromInt(MaxLifePoint)));
	SelectedCharacterCurrentLifePoint->SetText(FText::FromString(FString::FromInt(CurrentLifePoint)));

	SelectedCharacterLifePointMarker->SetText(FText::FromStringTable("/Game/DATATABLE/BattleWidgetTranslateData.BattleWidgetTranslateData", "SelectedCharacterLifePointMarker"));
	SelectedCharacterAmmoMarker->SetText(FText::FromStringTable("/Game/DATATABLE/BattleWidgetTranslateData.BattleWidgetTranslateData", "SelectedCharacterAmmoMarker"));
	SelectedCharacterHPSlash->SetText(FText::FromString(TEXT("/")));
	SelectedCharacterAmmoSlash->SetText(FText::FromString(TEXT("/")));

	SelectedCharacterSkillMarker1->SetText(FText::FromString(TEXT("1")));
	SelectedCharacterSkillMarker2->SetText(FText::FromString(TEXT("2")));
	SelectedCharacterSkillMarker3->SetText(FText::FromString(TEXT("4")));
	SelectedCharacterSkillMarker4->SetText(FText::FromString(TEXT("5")));
	SelectedCharacterSkillMarker5->SetText(FText::FromString(TEXT("3")));

	if (AttackButton->Visibility == ESlateVisibility::Hidden)
	{
		AttackButton->SetVisibility(ESlateVisibility::Visible);
		Skill1Button->SetVisibility(ESlateVisibility::Visible);
		ReloadButton->SetVisibility(ESlateVisibility::Visible);
		CoverButton->SetVisibility(ESlateVisibility::Visible);
		Skill2Button->SetVisibility(ESlateVisibility::Visible);
	}
}

void UBattleWidget::UpdateWiget_TargetCharacterPanel(class ASquadCharacter* TargetChar)
{
	FString TempPer = "%";

	FText name = Cast<AEnemySquadCharacter>(TargetChar)->GetEnemyCharacterBrunchName();
	float MaxAmmo = TargetChar->MaxAmmo;
	float CurrentAmmo = TargetChar->CurrentAmmo;
	float MaxLifePoint = TargetChar->MaxLifePoint;
	float CurrentLifePoint = TargetChar->LifePoint;

	TargetNameBlock->SetText(name);	
	TargetCharacterMaxAmmo->SetText(FText::FromString(FString::FromInt(MaxAmmo)));
	TargetCharacterCurrentAmmo->SetText(FText::FromString(FString::FromInt(CurrentAmmo)));
	TargetCharacterMaxLifePoint->SetText(FText::FromString(FString::FromInt(MaxLifePoint)));
	TargetCharacterCurrentLifePoint->SetText(FText::FromString(FString::FromInt(CurrentLifePoint)));
}

void UBattleWidget::UpdateWiget_TargetCharacterStatPanel(class ASquadCharacter* TargetChar, class ASquadCharacter* SelectedChar)
{
	FString TempPer = "%";
	
	int32 tempSkillAcc = 0;
	int32 tempSkillCrit = 0;
	uint8 tempSkillDamage = 0;
	uint8 tempSKillHit = 0;

	if (SelectedChar->StateEnum == EStateEnum::SE_Shot) {
		tempSkillDamage = Cast<APlayerSquadCharacter>(SelectedChar)->CharacterStat->GetWeaponDamage();
		tempSKillHit = Cast<APlayerSquadCharacter>(SelectedChar)->CharacterStat->GetWeaponFireCount();
	}
	else if (SelectedChar->StateEnum == EStateEnum::SE_Skill1) {
		auto SkillData = Cast<APlayerSquadCharacter>(SelectedChar)->CharacterSkillComp->Get_Skill1Data();
		
		tempSkillAcc = SkillData->Special_ability_1_UsedCharacterAccurancyrateCorrectionValue;
		tempSkillCrit = SkillData->Special_ability_2_UsedCharacterCriticalCorrectionValue;

		tempSkillDamage = SkillData->SkillDamage;
		tempSKillHit = SkillData->SkillBulletCount;
	}
	else if (SelectedChar->StateEnum == EStateEnum::SE_Skill2) {
		auto SkillData = Cast<APlayerSquadCharacter>(SelectedChar)->CharacterSkillComp->Get_Skill2Data();

		tempSkillAcc = SkillData->Special_ability_1_UsedCharacterAccurancyrateCorrectionValue;
		tempSkillCrit = SkillData->Special_ability_2_UsedCharacterCriticalCorrectionValue;

		tempSkillDamage = SkillData->SkillDamage;
		tempSKillHit = SkillData->SkillBulletCount;
	}

	TargetCharacterEvasion->SetText(FText::FromString(FString::FromInt(TargetChar->Evasion) + TempPer));
	TargetCharacterDefense->SetText(FText::FromString(FString::FromInt(TargetChar->Defense) + TempPer));
	TargetCharacterAccurancy->SetText(FText::FromString(FString::FromInt(SelectedChar->CharacterStat->GetWeaponAccuracy() + tempSkillAcc) + TempPer));
	TargetCharacterCritical->SetText(FText::FromString(FString::FromInt(SelectedChar->CharacterStat->GetWeaponCritical() + tempSkillCrit) + TempPer));
	TargetCharacterDamage->SetText(FText::FromString(FString::FromInt(TargetChar->Damage)));
	TargetCharacterHit->SetText(FText::FromString(FString::FromInt(TargetChar->FireCount)));
	TargetCharacterMaxDamage->SetText(FText::FromString(FString::FromInt(TargetChar->Damage * TargetChar->FireCount)));
	

	TargetCharacterEvasionMarker->SetText(FText::FromStringTable("/Game/DATATABLE/BattleWidgetTranslateData.BattleWidgetTranslateData", "TargetCharacterEVA"));
	TargetCharacterDefenseMarker->SetText(FText::FromStringTable("/Game/DATATABLE/BattleWidgetTranslateData.BattleWidgetTranslateData", "TargetCharacterDEF"));

	TargetCharacterAccurancyMarker->SetText(FText::FromString("+ACC"));
	TargetCharacterCriticalMarker->SetText(FText::FromString("+Crit"));
	TargetCharacterDamageMarker->SetText(FText::FromString("DMG"));
	TargetCharacterHitMarker->SetText(FText::FromString("HIT"));
	


	TargetCharacterLifePointMarker->SetText(FText::FromStringTable("/Game/DATATABLE/BattleWidgetTranslateData.BattleWidgetTranslateData", "TargetCharacterLifepointMarker"));
	TargetCharacterAmmoMarker->SetText(FText::FromStringTable("/Game/DATATABLE/BattleWidgetTranslateData.BattleWidgetTranslateData", "TargetCharacterAmmoMarker"));
	TargetCharacterHPSlash->SetText(FText::FromString(TEXT("/")));
	TargetCharacterAmmoSlash->SetText(FText::FromString(TEXT("/")));

	TargetCharacterTotalAccurancy->SetText(FText::FromString(FString::FromInt(SelectedChar->CharacterStat->GetWeaponAccuracy() + tempSkillAcc - TargetChar->Evasion) + TempPer));
	TargetCharacterTotalAccurancyMarker->SetText(FText::FromStringTable("/Game/DATATABLE/BattleWidgetTranslateData.BattleWidgetTranslateData", "SelectedCharacterACC"));
	TargetCharacterTotalCritical->SetText(FText::FromString(FString::FromInt(SelectedChar->CharacterStat->GetWeaponCritical() + tempSkillCrit - TargetChar->Defense) + TempPer));
	TargetCharacterTotalCriticalMarker->SetText(FText::FromStringTable("/Game/DATATABLE/BattleWidgetTranslateData.BattleWidgetTranslateData", "SelectedCharacterCrit"));
	TargetCharacterMaxDamage->SetText(FText::FromString(FString::FromInt(tempSkillDamage * tempSKillHit)));
	TargetCharacterMaxDamageMarker->SetText(FText::FromStringTable("/Game/DATATABLE/BattleWidgetTranslateData.BattleWidgetTranslateData", "TargetCharacterMaxDamage"));

}

void UBattleWidget::UpdateCharacterInfoText_Right2(float Acc, float Crit, float Damage, float Hitcount, float MaxDamage, float Evasion, float Defense ,class ASquadCharacter* Char)
{
	FString TempPer = "%";

	SelectedCharacterAccuracy->SetText(FText::FromString(FString::FromInt(Acc) + TempPer));
	SelectedCharacterCritical->SetText(FText::FromString(FString::FromInt(Crit) + TempPer));
	SelectedCharacterDamage->SetText(FText::FromString(FString::FromInt(Damage)));
	SelectedCharacterHitCount->SetText(FText::FromString(FString::FromInt(Hitcount)));
	SelectedCharacterMaxDamage->SetText(FText::FromString(FString::FromInt(MaxDamage)));
	SelectedCharacterEvasion->SetText(FText::FromString(FString::FromInt(Evasion) + TempPer));
	SelectedCharacterDefense->SetText(FText::FromString(FString::FromInt(Defense) + TempPer));

	FText::FromStringTable("/Game/DATATABLE/BattleWidgetTranslateData.BattleWidgetTranslateData", "SelectedCharacterSkillCooldown");
	SelectedCharacterAccuracyMarker->SetText(FText::FromStringTable("/Game/DATATABLE/BattleWidgetTranslateData.BattleWidgetTranslateData", "SelectedCharacterACC"));
	SelectedCharacterCriticalMarker->SetText(FText::FromStringTable("/Game/DATATABLE/BattleWidgetTranslateData.BattleWidgetTranslateData", "SelectedCharacterCrit"));
	SelectedCharacterMaxDamageMarker->SetText(FText::FromString(TEXT("MAX DMG")));
	SelectedCharacterDamageMarker->SetText(FText::FromStringTable("/Game/DATATABLE/BattleWidgetTranslateData.BattleWidgetTranslateData", "SelectedCharacterDamage"));
	SelectedCharacterHitCountMarker->SetText(FText::FromStringTable("/Game/DATATABLE/BattleWidgetTranslateData.BattleWidgetTranslateData", "SelectedCharacterHit"));
	SelectedCharacterEvasionMarker->SetText(FText::FromStringTable("/Game/DATATABLE/BattleWidgetTranslateData.BattleWidgetTranslateData", "SelectedCharacterEVA"));
	SelectedCharacterDefenseMarker->SetText(FText::FromStringTable("/Game/DATATABLE/BattleWidgetTranslateData.BattleWidgetTranslateData", "SelectedCharacterDEF"));


}

void UBattleWidget::UpdateCharacterDamageGraph(class ASquadCharacter* Char)
{
	FString TempPer = "%";

	FText ST_DMGGridMarker = FText::FromStringTable("/Game/DATATABLE/TranslateData.TranslateData", "BattleWidget.DamageAnalysis.DMG");
	FText ST_PERGridMarker = FText::FromStringTable("/Game/DATATABLE/TranslateData.TranslateData", "BattleWidget.DamageAnalysis.PER");
	FText ST_CritGridMarker = FText::FromStringTable("/Game/DATATABLE/TranslateData.TranslateData", "BattleWidget.DamageAnalysis.Crit");


	SelectedCharacterDMGGridMarker->SetText(ST_DMGGridMarker);
	SelectedCharacterACCGridMarker->SetText(ST_PERGridMarker);
	SelectedCharacterCritGridMarker->SetText(ST_CritGridMarker);



	SelectedCharacterDamage0GridMarker->SetText(FText::FromString(TEXT("0")));
	SelectedCharacterDamage1GridMarker->SetText(FText::FromString(TEXT("1")));
	SelectedCharacterDamage2GridMarker->SetText(FText::FromString(TEXT("2")));
	SelectedCharacterDamage3GridMarker->SetText(FText::FromString(TEXT("3")));
	SelectedCharacterDamage4GridMarker->SetText(FText::FromString(TEXT("4")));
	SelectedCharacterDamage5GridMarker->SetText(FText::FromString(TEXT("5")));
	SelectedCharacterDamage6GridMarker->SetText(FText::FromString(TEXT("6")));
	SelectedCharacterDamage7GridMarker->SetText(FText::FromString(TEXT("7")));
	SelectedCharacterDamage8GridMarker->SetText(FText::FromString(TEXT("8")));
	SelectedCharacterDamage9GridMarker->SetText(FText::FromString(TEXT("9")));
	SelectedCharacterDamage10GridMarker->SetText(FText::FromString(TEXT("10")));

	APlayerSquadCharacter* PChar = Cast<APlayerSquadCharacter>(Char);

	if(PChar->StateEnum == EStateEnum::SE_Shot) {
		SelectedCharacterDamage0perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->Damage_distribution[0]) + TempPer));
		SelectedCharacterDamage1perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->Damage_distribution[1]) + TempPer));
		SelectedCharacterDamage2perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->Damage_distribution[2]) + TempPer));
		SelectedCharacterDamage3perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->Damage_distribution[3]) + TempPer));
		SelectedCharacterDamage4perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->Damage_distribution[4]) + TempPer));
		SelectedCharacterDamage5perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->Damage_distribution[5]) + TempPer));
		SelectedCharacterDamage6perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->Damage_distribution[6]) + TempPer));
		SelectedCharacterDamage7perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->Damage_distribution[7]) + TempPer));
		SelectedCharacterDamage8perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->Damage_distribution[8]) + TempPer));
		SelectedCharacterDamage9perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->Damage_distribution[9]) + TempPer));
		SelectedCharacterDamage10perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->Damage_distribution[10]) + TempPer));

		SetDamageMarkerColor(SelectedCharacterDamage0perGridMarker, PChar->Damage_distribution[0], PChar->MaxDamage_InDamageDis, PChar->Damage_distribution_float[0]);
		SetDamageMarkerColor(SelectedCharacterDamage1perGridMarker, PChar->Damage_distribution[1], PChar->MaxDamage_InDamageDis, PChar->Damage_distribution_float[1]);
		SetDamageMarkerColor(SelectedCharacterDamage2perGridMarker, PChar->Damage_distribution[2], PChar->MaxDamage_InDamageDis, PChar->Damage_distribution_float[2]);
		SetDamageMarkerColor(SelectedCharacterDamage3perGridMarker, PChar->Damage_distribution[3], PChar->MaxDamage_InDamageDis, PChar->Damage_distribution_float[3]);
		SetDamageMarkerColor(SelectedCharacterDamage4perGridMarker, PChar->Damage_distribution[4], PChar->MaxDamage_InDamageDis, PChar->Damage_distribution_float[4]);
		SetDamageMarkerColor(SelectedCharacterDamage5perGridMarker, PChar->Damage_distribution[5], PChar->MaxDamage_InDamageDis, PChar->Damage_distribution_float[5]);
		SetDamageMarkerColor(SelectedCharacterDamage6perGridMarker, PChar->Damage_distribution[6], PChar->MaxDamage_InDamageDis, PChar->Damage_distribution_float[6]);
		SetDamageMarkerColor(SelectedCharacterDamage7perGridMarker, PChar->Damage_distribution[7], PChar->MaxDamage_InDamageDis, PChar->Damage_distribution_float[7]);
		SetDamageMarkerColor(SelectedCharacterDamage8perGridMarker, PChar->Damage_distribution[8], PChar->MaxDamage_InDamageDis, PChar->Damage_distribution_float[8]);
		SetDamageMarkerColor(SelectedCharacterDamage9perGridMarker, PChar->Damage_distribution[9], PChar->MaxDamage_InDamageDis, PChar->Damage_distribution_float[9]);
		SetDamageMarkerColor(SelectedCharacterDamage10perGridMarker, PChar->Damage_distribution[10], PChar->MaxDamage_InDamageDis, PChar->Damage_distribution_float[10]);

		SelectedCharacterCritperGridMarker->SetText(FText::FromString(FString::FromInt((Cast<APlayerSquadCharacter>(Char)->CharacterStat->GetCharacterCriticalCorrectionValue() + Cast<APlayerSquadCharacter>(Char)->CharacterStat->GetWeaponCritical()) * Cast<APlayerSquadCharacter>(Char)->CharacterStat->GetWeaponFireCount()) + TempPer));
	}
	else if (PChar->StateEnum == EStateEnum::SE_Skill1) {
		SelectedCharacterDamage0perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->SkillDamage_distribution[0]) + TempPer));
		SelectedCharacterDamage1perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->SkillDamage_distribution[1]) + TempPer));
		SelectedCharacterDamage2perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->SkillDamage_distribution[2]) + TempPer));
		SelectedCharacterDamage3perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->SkillDamage_distribution[3]) + TempPer));
		SelectedCharacterDamage4perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->SkillDamage_distribution[4]) + TempPer));
		SelectedCharacterDamage5perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->SkillDamage_distribution[5]) + TempPer));
		SelectedCharacterDamage6perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->SkillDamage_distribution[6]) + TempPer));
		SelectedCharacterDamage7perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->SkillDamage_distribution[7]) + TempPer));
		SelectedCharacterDamage8perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->SkillDamage_distribution[8]) + TempPer));
		SelectedCharacterDamage9perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->SkillDamage_distribution[9]) + TempPer));
		SelectedCharacterDamage10perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->SkillDamage_distribution[10]) + TempPer));

		SetDamageMarkerColor(SelectedCharacterDamage0perGridMarker, PChar->SkillDamage_distribution[0], PChar->SkillMaxDamage_InDamageDis, PChar->SkillDamage_distribution_float[0]);
		SetDamageMarkerColor(SelectedCharacterDamage1perGridMarker, PChar->SkillDamage_distribution[1], PChar->SkillMaxDamage_InDamageDis, PChar->SkillDamage_distribution_float[1]);
		SetDamageMarkerColor(SelectedCharacterDamage2perGridMarker, PChar->SkillDamage_distribution[2], PChar->SkillMaxDamage_InDamageDis, PChar->SkillDamage_distribution_float[2]);
		SetDamageMarkerColor(SelectedCharacterDamage3perGridMarker, PChar->SkillDamage_distribution[3], PChar->SkillMaxDamage_InDamageDis, PChar->SkillDamage_distribution_float[3]);
		SetDamageMarkerColor(SelectedCharacterDamage4perGridMarker, PChar->SkillDamage_distribution[4], PChar->SkillMaxDamage_InDamageDis, PChar->SkillDamage_distribution_float[4]);
		SetDamageMarkerColor(SelectedCharacterDamage5perGridMarker, PChar->SkillDamage_distribution[5], PChar->SkillMaxDamage_InDamageDis, PChar->SkillDamage_distribution_float[5]);
		SetDamageMarkerColor(SelectedCharacterDamage6perGridMarker, PChar->SkillDamage_distribution[6], PChar->SkillMaxDamage_InDamageDis, PChar->SkillDamage_distribution_float[6]);
		SetDamageMarkerColor(SelectedCharacterDamage7perGridMarker, PChar->SkillDamage_distribution[7], PChar->SkillMaxDamage_InDamageDis, PChar->SkillDamage_distribution_float[7]);
		SetDamageMarkerColor(SelectedCharacterDamage8perGridMarker, PChar->SkillDamage_distribution[8], PChar->SkillMaxDamage_InDamageDis, PChar->SkillDamage_distribution_float[8]);
		SetDamageMarkerColor(SelectedCharacterDamage9perGridMarker, PChar->SkillDamage_distribution[9], PChar->SkillMaxDamage_InDamageDis, PChar->SkillDamage_distribution_float[9]);
		SetDamageMarkerColor(SelectedCharacterDamage10perGridMarker, PChar->SkillDamage_distribution[10], PChar->SkillMaxDamage_InDamageDis, PChar->SkillDamage_distribution_float[10]);


		float tempCrit = Cast<APlayerSquadCharacter>(Char)->CharacterStat->GetCharacterCriticalCorrectionValue() + Cast<APlayerSquadCharacter>(Char)->CharacterStat->GetWeaponCritical() + Cast<APlayerSquadCharacter>(Char)->CharacterSkillComp->Get_Skill1Data()->Special_ability_2_UsedCharacterCriticalCorrectionValue;
		if (tempCrit >= 0)
			SelectedCharacterCritperGridMarker->SetText(FText::FromString(FString::FromInt((Cast<APlayerSquadCharacter>(Char)->CharacterStat->GetCharacterCriticalCorrectionValue() + Cast<APlayerSquadCharacter>(Char)->CharacterStat->GetWeaponCritical() + Cast<APlayerSquadCharacter>(Char)->CharacterSkillComp->Get_Skill1Data()->Special_ability_2_UsedCharacterCriticalCorrectionValue) * Cast<APlayerSquadCharacter>(Char)->CharacterStat->GetWeaponFireCount()) + TempPer));
		else
			SelectedCharacterCritperGridMarker->SetText(FText::FromString(TEXT("0") + TempPer));		
	}
	else if (PChar->StateEnum == EStateEnum::SE_Skill2) {
		SelectedCharacterDamage0perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->SkillDamage_distribution[0]) + TempPer));
		SelectedCharacterDamage1perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->SkillDamage_distribution[1]) + TempPer));
		SelectedCharacterDamage2perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->SkillDamage_distribution[2]) + TempPer));
		SelectedCharacterDamage3perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->SkillDamage_distribution[3]) + TempPer));
		SelectedCharacterDamage4perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->SkillDamage_distribution[4]) + TempPer));
		SelectedCharacterDamage5perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->SkillDamage_distribution[5]) + TempPer));
		SelectedCharacterDamage6perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->SkillDamage_distribution[6]) + TempPer));
		SelectedCharacterDamage7perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->SkillDamage_distribution[7]) + TempPer));
		SelectedCharacterDamage8perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->SkillDamage_distribution[8]) + TempPer));
		SelectedCharacterDamage9perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->SkillDamage_distribution[9]) + TempPer));
		SelectedCharacterDamage10perGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->SkillDamage_distribution[10]) + TempPer));

		SetDamageMarkerColor(SelectedCharacterDamage0perGridMarker, PChar->SkillDamage_distribution[0], PChar->SkillMaxDamage_InDamageDis, PChar->SkillDamage_distribution_float[0]);
		SetDamageMarkerColor(SelectedCharacterDamage1perGridMarker, PChar->SkillDamage_distribution[1], PChar->SkillMaxDamage_InDamageDis, PChar->SkillDamage_distribution_float[1]);
		SetDamageMarkerColor(SelectedCharacterDamage2perGridMarker, PChar->SkillDamage_distribution[2], PChar->SkillMaxDamage_InDamageDis, PChar->SkillDamage_distribution_float[2]);
		SetDamageMarkerColor(SelectedCharacterDamage3perGridMarker, PChar->SkillDamage_distribution[3], PChar->SkillMaxDamage_InDamageDis, PChar->SkillDamage_distribution_float[3]);
		SetDamageMarkerColor(SelectedCharacterDamage4perGridMarker, PChar->SkillDamage_distribution[4], PChar->SkillMaxDamage_InDamageDis, PChar->SkillDamage_distribution_float[4]);
		SetDamageMarkerColor(SelectedCharacterDamage5perGridMarker, PChar->SkillDamage_distribution[5], PChar->SkillMaxDamage_InDamageDis, PChar->SkillDamage_distribution_float[5]);
		SetDamageMarkerColor(SelectedCharacterDamage6perGridMarker, PChar->SkillDamage_distribution[6], PChar->SkillMaxDamage_InDamageDis, PChar->SkillDamage_distribution_float[6]);
		SetDamageMarkerColor(SelectedCharacterDamage7perGridMarker, PChar->SkillDamage_distribution[7], PChar->SkillMaxDamage_InDamageDis, PChar->SkillDamage_distribution_float[7]);
		SetDamageMarkerColor(SelectedCharacterDamage8perGridMarker, PChar->SkillDamage_distribution[8], PChar->SkillMaxDamage_InDamageDis, PChar->SkillDamage_distribution_float[8]);
		SetDamageMarkerColor(SelectedCharacterDamage9perGridMarker, PChar->SkillDamage_distribution[9], PChar->SkillMaxDamage_InDamageDis, PChar->SkillDamage_distribution_float[9]);
		SetDamageMarkerColor(SelectedCharacterDamage10perGridMarker, PChar->SkillDamage_distribution[10], PChar->SkillMaxDamage_InDamageDis, PChar->SkillDamage_distribution_float[10]);

		float tempCrit = Cast<APlayerSquadCharacter>(Char)->CharacterStat->GetCharacterCriticalCorrectionValue() + Cast<APlayerSquadCharacter>(Char)->CharacterStat->GetWeaponCritical() + Cast<APlayerSquadCharacter>(Char)->CharacterSkillComp->Get_Skill2Data()->Special_ability_2_UsedCharacterCriticalCorrectionValue;
		if(tempCrit >= 0)
			SelectedCharacterCritperGridMarker->SetText(FText::FromString(FString::FromInt((Cast<APlayerSquadCharacter>(Char)->CharacterStat->GetCharacterCriticalCorrectionValue() + Cast<APlayerSquadCharacter>(Char)->CharacterStat->GetWeaponCritical() + Cast<APlayerSquadCharacter>(Char)->CharacterSkillComp->Get_Skill2Data()->Special_ability_2_UsedCharacterCriticalCorrectionValue) * Cast<APlayerSquadCharacter>(Char)->CharacterStat->GetWeaponFireCount()) + TempPer));
		else
			SelectedCharacterCritperGridMarker->SetText(FText::FromString(TEXT("0") + TempPer));
	}

	//SelectedCharacterCritperGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->MaxDamage_InDamageDis) + TempPer));
	//SelectedCharacterCritperGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->CharacterStat->GetWeaponCritical()) + TempPer));
	SelectedCharacterBattleTable->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.0f));

	SelectedCharacterSkillGraphBlackBackground->SetColorAndOpacity(FLinearColor(0.01f, 0.01f, 0.005f, 1.0f));
	SelectedCharacterSkillGraphWhiteBackground->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.0f));
	SelectedCharacterSkillGraphMarker->SetVisibility(ESlateVisibility::Visible);
}

void UBattleWidget::UpdateCharacterDamageGraph_Blank()
{
	FString TempPer = "%";

	FText ST_DMGGridMarker = FText::FromStringTable("/Game/DATATABLE/TranslateData.TranslateData", "BattleWidget.DamageAnalysis.DMG");
	FText ST_PERGridMarker = FText::FromStringTable("/Game/DATATABLE/TranslateData.TranslateData", "BattleWidget.DamageAnalysis.PER");
	FText ST_CritGridMarker = FText::FromStringTable("/Game/DATATABLE/TranslateData.TranslateData", "BattleWidget.DamageAnalysis.Crit");


	SelectedCharacterDMGGridMarker->SetText(ST_DMGGridMarker);
	SelectedCharacterACCGridMarker->SetText(ST_PERGridMarker);
	SelectedCharacterCritGridMarker->SetText(ST_CritGridMarker);



	SelectedCharacterDamage0GridMarker->SetText(FText::FromString(TEXT("0")));
	SelectedCharacterDamage1GridMarker->SetText(FText::FromString(TEXT("1")));
	SelectedCharacterDamage2GridMarker->SetText(FText::FromString(TEXT("2")));
	SelectedCharacterDamage3GridMarker->SetText(FText::FromString(TEXT("3")));
	SelectedCharacterDamage4GridMarker->SetText(FText::FromString(TEXT("4")));
	SelectedCharacterDamage5GridMarker->SetText(FText::FromString(TEXT("5")));
	SelectedCharacterDamage6GridMarker->SetText(FText::FromString(TEXT("6")));
	SelectedCharacterDamage7GridMarker->SetText(FText::FromString(TEXT("7")));
	SelectedCharacterDamage8GridMarker->SetText(FText::FromString(TEXT("8")));
	SelectedCharacterDamage9GridMarker->SetText(FText::FromString(TEXT("9")));
	SelectedCharacterDamage10GridMarker->SetText(FText::FromString(TEXT("10")));

	//APlayerSquadCharacter* PChar = Cast<APlayerSquadCharacter>(Char);	

	//SelectedCharacterCritperGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->MaxDamage_InDamageDis) + TempPer));
	//SelectedCharacterCritperGridMarker->SetText(FText::FromString(FString::FromInt(Cast<APlayerSquadCharacter>(Char)->CharacterStat->GetWeaponCritical()) + TempPer));
	SelectedCharacterBattleTable->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.0f));

	SelectedCharacterSkillGraphBlackBackground->SetColorAndOpacity(FLinearColor(0.01f, 0.01f, 0.005f, 1.0f));
	SelectedCharacterSkillGraphWhiteBackground->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.0f));
	SelectedCharacterSkillGraphMarker->SetVisibility(ESlateVisibility::Visible);
}

void UBattleWidget::Updata_CharacterSkill_Info(class APlayerSquadCharacter* Char, int32 skillNumber)
{
	Char->CharacterSkillComp->Get_Skill1Data();
	SelectedCharacterSkillName->SetText(FText::FromString(TEXT("")));
	SelectedCharacterSkillAccurancy->SetText(FText::FromString(TEXT("")));
	SelectedCharacterSkillCritical->SetText(FText::FromString(TEXT("")));
}

void UBattleWidget::ClearCharacterInfoText_Left()
{
	CurrentCharacter->SetText(FText::FromString(TEXT("")));
	SelectedCharacterMaxAmmo->SetText(FText::FromString(TEXT("")));
	SelectedCharacterCurrentAmmo->SetText(FText::FromString(TEXT("")));
	SelectedCharacterMaxLifePoint->SetText(FText::FromString(TEXT("")));
	SelectedCharacterCurrentLifePoint->SetText(FText::FromString(TEXT("")));

	SelectedCharacterLifePointMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterAmmoMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterHPSlash->SetText(FText::FromString(TEXT("")));
	SelectedCharacterAmmoSlash->SetText(FText::FromString(TEXT("")));

	SelectedCharacterSkillMarker1->SetText(FText::FromString(TEXT("")));
	SelectedCharacterSkillMarker2->SetText(FText::FromString(TEXT("")));
	SelectedCharacterSkillMarker3->SetText(FText::FromString(TEXT("")));
	SelectedCharacterSkillMarker4->SetText(FText::FromString(TEXT("")));
	SelectedCharacterSkillMarker5->SetText(FText::FromString(TEXT("")));

	if (AttackButton->Visibility == ESlateVisibility::Visible)
	{
		AttackButton->SetVisibility(ESlateVisibility::Hidden);
		Skill1Button->SetVisibility(ESlateVisibility::Hidden);
		ReloadButton->SetVisibility(ESlateVisibility::Hidden);
		CoverButton->SetVisibility(ESlateVisibility::Hidden);
		Skill2Button->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UBattleWidget::ClearCharacterInfoText_Right()
{
	TargetNameBlock->SetText(FText::FromString(TEXT("")));
	TargetCharacterMaxAmmo->SetText(FText::FromString(TEXT("")));
	TargetCharacterCurrentAmmo->SetText(FText::FromString(TEXT("")));
	TargetCharacterMaxLifePoint->SetText(FText::FromString(TEXT("")));
	TargetCharacterCurrentLifePoint->SetText(FText::FromString(TEXT("")));

	TargetCharacterLifePointMarker->SetText(FText::FromString(TEXT("")));
	TargetCharacterAmmoMarker->SetText(FText::FromString(TEXT("")));
	TargetCharacterHPSlash->SetText(FText::FromString(TEXT("")));
	TargetCharacterAmmoSlash->SetText(FText::FromString(TEXT("")));

	TargetCharacterEvasion->SetText(FText::FromString(TEXT("")));
	TargetCharacterDefense->SetText(FText::FromString(TEXT("")));
	TargetCharacterAccurancy->SetText(FText::FromString(TEXT("")));
	TargetCharacterCritical-> SetText(FText::FromString(TEXT("")));
	TargetCharacterDamage->SetText(FText::FromString(TEXT("")));
	TargetCharacterHit->SetText(FText::FromString(TEXT("")));
	TargetCharacterMaxDamage->SetText(FText::FromString(TEXT("")));

	TargetCharacterEvasionMarker->SetText(FText::FromString(TEXT("")));
	TargetCharacterDefenseMarker->SetText(FText::FromString(TEXT("")));
	TargetCharacterAccurancyMarker->SetText(FText::FromString(TEXT("")));
	TargetCharacterCriticalMarker->SetText(FText::FromString(TEXT("")));
	TargetCharacterDamageMarker->SetText(FText::FromString(TEXT("")));
	TargetCharacterHitMarker->SetText(FText::FromString(TEXT("")));
	TargetCharacterMaxDamageMarker->SetText(FText::FromString(TEXT("")));

	TargetCharacterTotalAccurancy->SetText(FText::FromString(TEXT("")));
	TargetCharacterTotalAccurancyMarker->SetText(FText::FromString(TEXT("")));
	TargetCharacterTotalCritical->SetText(FText::FromString(TEXT("")));
	TargetCharacterTotalCriticalMarker->SetText(FText::FromString(TEXT("")));

	SelectedCharacterAccuracy->SetText(FText::FromString(TEXT("")));
	SelectedCharacterCritical->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDamage->SetText(FText::FromString(TEXT("")));
	SelectedCharacterHitCount->SetText(FText::FromString(TEXT("")));
	SelectedCharacterMaxDamage->SetText(FText::FromString(TEXT("")));
	SelectedCharacterEvasion->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDefense->SetText(FText::FromString(TEXT("")));

	SelectedCharacterAccuracyMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterCriticalMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterMaxDamageMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDamageMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterHitCountMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterEvasionMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDefenseMarker->SetText(FText::FromString(TEXT("")));

	SelectedCharacterDMGGridMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterACCGridMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterCritGridMarker->SetText(FText::FromString(TEXT("")));

	SelectedCharacterDamage0GridMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDamage1GridMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDamage2GridMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDamage3GridMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDamage4GridMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDamage5GridMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDamage6GridMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDamage7GridMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDamage8GridMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDamage9GridMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDamage10GridMarker->SetText(FText::FromString(TEXT("")));

	SelectedCharacterDamage0perGridMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDamage1perGridMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDamage2perGridMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDamage3perGridMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDamage4perGridMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDamage5perGridMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDamage6perGridMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDamage7perGridMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDamage8perGridMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDamage9perGridMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterDamage10perGridMarker->SetText(FText::FromString(TEXT("")));


	SelectedCharacterCritperGridMarker->SetText(FText::FromString(TEXT("")));
	SelectedCharacterBattleTable->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.0f));
	SelectedCharacterSkillGraphBlackBackground->SetColorAndOpacity(FLinearColor(0.01f, 0.01f, 0.005f, 0.0f));
	SelectedCharacterSkillGraphWhiteBackground->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.0f));
	SelectedCharacterSkillGraphMarker->SetVisibility(ESlateVisibility::Hidden);
}

void UBattleWidget::SetDamageMarkerColor(UTextBlock* DamageText, float per, float maxper, float per_float)
{
	
	FLinearColor Green(0.f, 1.f, 0.f, 1.f);
	FLinearColor Grass(0.5f, 1.f, 0.f, 1.f);
	FLinearColor Yellow(1.f, 1.f, 0.f, 1.f);
	FLinearColor Orange(1.f, 0.5f, 0.f, 1.f);
	FLinearColor Red(1.f, 0.f, 0.f, 1.f);
	FLinearColor White(1.f, 1.f, 1.f, 1.f);

	
	float f_temp = (per / maxper) * 100;
	int32 i_temp = f_temp;
	
	if(i_temp >= 80 && i_temp <= 100) //80 100
		DamageText->SetColorAndOpacity(Green); // 초록
	else if(i_temp >= 60 && i_temp < 80) //60 80미만
		DamageText->SetColorAndOpacity(Grass); // 연두색
	else if(i_temp >= 40 && i_temp < 60) // 40 60미만
		DamageText->SetColorAndOpacity(Yellow); // 노란색
	else if(i_temp >= 20 && i_temp < 40) // 20 40미만
		DamageText->SetColorAndOpacity(Orange); // 주황색
	else if (i_temp >= 0 && i_temp < 20) // 0 20미만
		DamageText->SetColorAndOpacity(Red); // 빨강색

	
	if(per_float == 0.f) {
		DamageText->SetText(FText::FromString(TEXT("-")));
		DamageText->SetColorAndOpacity(White);
	}
	

}

void UBattleWidget::Set_BattleWidgetOpacity(float val)
{
	//CurrentCharacter->SetOpacity(val);

	SelectedCharacterSkillMarker1->SetOpacity(val);
	SelectedCharacterSkillMarker2->SetOpacity(val);
	SelectedCharacterSkillMarker3->SetOpacity(val);
	SelectedCharacterSkillMarker4->SetOpacity(val);
	SelectedCharacterSkillMarker5->SetOpacity(val);

	/*
	SelectedCharacterMaxAmmo->SetOpacity(val);
	SelectedCharacterCurrentAmmo->SetOpacity(val);
	SelectedCharacterMaxLifePoint->SetOpacity(val);
	SelectedCharacterCurrentLifePoint->SetOpacity(val);

	SelectedCharacterHPSlash->SetOpacity(val);
	SelectedCharacterAmmoSlash->SetOpacity(val);

	SelectedCharacterAmmoMarker->SetOpacity(val);
	SelectedCharacterLifePointMarker->SetOpacity(val);

	//AttackButton->SetRenderOpacity(val);
	//Skill1Button->SetRenderOpacity(val);
	//ReloadButton->SetRenderOpacity(val);
	//CoverButton->SetRenderOpacity(val);
	//Skill2Button->SetRenderOpacity(val);

	TargetNameBlock->SetOpacity(val);
	TargetCharacterMaxAmmo->SetOpacity(val);
	TargetCharacterCurrentAmmo->SetOpacity(val);
	TargetCharacterMaxLifePoint->SetOpacity(val);
	TargetCharacterCurrentLifePoint->SetOpacity(val);

	TargetCharacterLifePointMarker->SetOpacity(val);
	TargetCharacterAmmoMarker->SetOpacity(val);
	TargetCharacterHPSlash->SetOpacity(val);
	TargetCharacterAmmoSlash->SetOpacity(val);

	TargetCharacterEvasion->SetOpacity(val);
	TargetCharacterDefense->SetOpacity(val);
	TargetCharacterAccurancy->SetOpacity(val);
	TargetCharacterCritical->SetOpacity(val);
	TargetCharacterDamage->SetOpacity(val);
	TargetCharacterHit->SetOpacity(val);
	TargetCharacterMaxDamage->SetOpacity(val);

	TargetCharacterEvasionMarker->SetOpacity(val);
	TargetCharacterDefenseMarker->SetOpacity(val);
	TargetCharacterAccurancyMarker->SetOpacity(val);
	TargetCharacterCriticalMarker->SetOpacity(val);
	TargetCharacterDamageMarker->SetOpacity(val);
	TargetCharacterHitMarker->SetOpacity(val);
	TargetCharacterMaxDamageMarker->SetOpacity(val);

	TargetCharacterTotalAccurancy->SetOpacity(val);
	TargetCharacterTotalAccurancyMarker->SetOpacity(val);
	TargetCharacterTotalCritical->SetOpacity(val);
	TargetCharacterTotalCriticalMarker->SetOpacity(val);

	SelectedCharacterAccuracy->SetOpacity(val);
	SelectedCharacterCritical->SetOpacity(val);
	SelectedCharacterDamage->SetOpacity(val);
	SelectedCharacterHitCount->SetOpacity(val);
	SelectedCharacterMaxDamage->SetOpacity(val);
	SelectedCharacterEvasion->SetOpacity(val);
	SelectedCharacterDefense->SetOpacity(val);

	SelectedCharacterAccuracyMarker->SetOpacity(val);
	SelectedCharacterCriticalMarker->SetOpacity(val);
	SelectedCharacterMaxDamageMarker->SetOpacity(val);
	SelectedCharacterDamageMarker->SetOpacity(val);
	SelectedCharacterHitCountMarker->SetOpacity(val);
	SelectedCharacterEvasionMarker->SetOpacity(val);
	SelectedCharacterDefenseMarker->SetOpacity(val);

	SelectedCharacterDMGGridMarker->SetOpacity(val);
	SelectedCharacterACCGridMarker->SetOpacity(val);
	SelectedCharacterCritGridMarker->SetOpacity(val);

	SelectedCharacterDamage0GridMarker->SetOpacity(val);
	SelectedCharacterDamage1GridMarker->SetOpacity(val);
	SelectedCharacterDamage2GridMarker->SetOpacity(val);
	SelectedCharacterDamage3GridMarker->SetOpacity(val);
	SelectedCharacterDamage4GridMarker->SetOpacity(val);
	SelectedCharacterDamage5GridMarker->SetOpacity(val);
	SelectedCharacterDamage6GridMarker->SetOpacity(val);
	SelectedCharacterDamage7GridMarker->SetOpacity(val);
	SelectedCharacterDamage8GridMarker->SetOpacity(val);
	SelectedCharacterDamage9GridMarker->SetOpacity(val);
	SelectedCharacterDamage10GridMarker->SetOpacity(val);

	SelectedCharacterDamage0perGridMarker->SetRenderOpacity(val);
	SelectedCharacterDamage1perGridMarker->SetRenderOpacity(val);
	SelectedCharacterDamage2perGridMarker->SetRenderOpacity(val);
	SelectedCharacterDamage3perGridMarker->SetRenderOpacity(val);
	SelectedCharacterDamage4perGridMarker->SetRenderOpacity(val);
	SelectedCharacterDamage5perGridMarker->SetRenderOpacity(val);
	SelectedCharacterDamage6perGridMarker->SetRenderOpacity(val);
	SelectedCharacterDamage7perGridMarker->SetRenderOpacity(val);
	SelectedCharacterDamage8perGridMarker->SetRenderOpacity(val);
	SelectedCharacterDamage9perGridMarker->SetRenderOpacity(val);
	SelectedCharacterDamage10perGridMarker->SetRenderOpacity(val);

	

	SelectedCharacterCritperGridMarker->SetOpacity(val);
	SelectedCharacterBattleTable->SetOpacity(val);
	SelectedCharacterSkillGraphBlackBackground->SetOpacity(val);
	SelectedCharacterSkillGraphWhiteBackground->SetOpacity(val);
	SelectedCharacterSkillGraphMarker->SetRenderOpacity(val);

	SelectedCharacterBattleTable->SetRenderOpacity(val);
	SelectedCharacterSkillGraphBlackBackground->SetRenderOpacity(val);
	SelectedCharacterSkillGraphWhiteBackground->SetRenderOpacity(val);
	SelectedCharacterSkillGraphMarker->SetRenderOpacity(val);

	BattlewidgetRightPanelBackground->SetRenderOpacity(val);
	BattlewidgetLeftPanelBackground->SetRenderOpacity(val);
	*/
}

void UBattleWidget::UpdateWidget_SkillPart(class APlayerSquadCharacter* Pcharacter , float SkillNum)
{
	FSkillValueList SkillData__;
	FSkillValueList* skillData;

	skillData = &SkillData__;

	if (SkillNum == 1)
		skillData = Pcharacter->CharacterSkillComp->Get_Skill1Data();
	else if (SkillNum == 2)
		skillData = Pcharacter->CharacterSkillComp->Get_Skill2Data();
	else if (SkillNum == 0)
		skillData = Pcharacter->CharacterSkillComp->Get_Skill0Data();
	else if (SkillNum == 3)
		skillData = Pcharacter->CharacterSkillComp->Get_Skill3Data();
	else if (SkillNum == 4)
		skillData = Pcharacter->CharacterSkillComp->Get_Skill4Data();


	
	

#define LOCTEXT_NAMESPACE "MyNamespace"

	
	//auto SAstring = FText::Format(LOCTEXT("AttributeText", "{0}"), "SkillAttribute"); // TEXT("%s", skillData->SkillAttribute));
					// FText::Format(LOCTEXT("CooldownText", " {0} : {1}"), ST_cooldownText, skillData->CooldownValue)
	//auto skillatrribute = skillData->SkillAttribute;

	
	FText ST_Skillname = FText::FromStringTable("/Game/DATATABLE/PlayerSkillTranslateData.PlayerSkillTranslateData", skillData->WeaponName+"."+skillData->BrunchName+".Skillname");
	FText ST_SkillAttributeName = FText::FromStringTable("/Game/DATATABLE/PlayerSkillTranslateData.PlayerSkillTranslateData", "SkillAttribute." + skillData->SkillAttribute);
	FText ST_SkillAttributeExplanation = FText::FromStringTable("/Game/DATATABLE/PlayerSkillTranslateData.PlayerSkillTranslateData", "SkillAttribute." + skillData->SkillAttribute + ".SkillAttributeExplanation");
	FText ST_Skillexplanation = FText::FromStringTable("/Game/DATATABLE/PlayerSkillTranslateData.PlayerSkillTranslateData", skillData->WeaponName + "." + skillData->BrunchName + ".Skillexplanation");

	FText ST_AccText = FText::FromStringTable("/Game/DATATABLE/BattleWidgetTranslateData.BattleWidgetTranslateData", "SelectedCharacterACC");
	FText ST_CritText = FText::FromStringTable("/Game/DATATABLE/BattleWidgetTranslateData.BattleWidgetTranslateData", "SelectedCharacterCrit");
	FText ST_cooldownText = FText::FromStringTable("/Game/DATATABLE/BattleWidgetTranslateData.BattleWidgetTranslateData", "SelectedCharacterSkillCooldown");

	SelectedCharacterSkillName->SetText(ST_Skillname);
	SelectedCharacterSkillAttribute->SetText(ST_SkillAttributeName);
	SelectedCharacterSkillAttributeEffect->SetText(ST_SkillAttributeExplanation);
	SelectedCharacterSkillExplanation->SetText(ST_Skillexplanation);

	SelectedCharacterSkillAccurancy->SetText(FText::Format(LOCTEXT("AccText", "{0}: {1}%"), ST_AccText, skillData->Special_ability_1_UsedCharacterAccurancyrateCorrectionValue));
	SelectedCharacterSkillCritical->SetText(FText::Format(LOCTEXT("AccText", "{0}: {1}%"), ST_CritText, skillData->Special_ability_2_UsedCharacterCriticalCorrectionValue));
	SelectedCharacterSkillCooldown->SetText(FText::Format(LOCTEXT("CooldownText", " {0} : {1}"), ST_cooldownText, skillData->CooldownValue));

	
	//SelectedCharacterSkillCooldown->SetText(FText::FromString("COOLDOWN: " + FString::FromInt(skillData->CooldownValue)));
	//auto text_cooldown = ;
	

#undef LOCTEXT_NAMESPACE

	//SelectedCharacterSkillExplanation->SetText(FText::FromString(skillData->SkillExplanation));





}

void UBattleWidget::ClearWidget_SkillPart()
{
	SelectedCharacterSkillName->SetText(FText::FromString(TEXT("")));
	SelectedCharacterSkillAttribute->SetText(FText::FromString(TEXT("")));
	SelectedCharacterSkillAttributeEffect->SetText(FText::FromString(TEXT("")));
	SelectedCharacterSkillAccurancy->SetText(FText::FromString(TEXT("")));
	SelectedCharacterSkillCritical->SetText(FText::FromString(TEXT("")));
	SelectedCharacterSkillCooldown->SetText(FText::FromString(TEXT("")));
	SelectedCharacterSkillExplanation->SetText(FText::FromString(TEXT("")));
}

void UBattleWidget::Set_BattleWidgetSkilliconOpacity(bool Active)
{	
	if(Active) {
		AttackButton->BackgroundColor = FLinearColor(1.f, 0.f, 0.f, 1.f);
		Skill0ButtonImage->SetColorAndOpacity(FLinearColor(0.9f, 0.9f, 0.9f, 1.0f));

		Skill1Button->BackgroundColor = FLinearColor(0.0f, 1.0f, 0.f, 1.f);
		Skill1ButtonImage->SetColorAndOpacity(FLinearColor(0.9f, 0.9f, 0.9f, 1.0f));

		Skill2Button->BackgroundColor = FLinearColor(0.f, 1.0f, 0.f, 1.f);
		Skill2ButtonImage->SetColorAndOpacity(FLinearColor(0.9f, 0.9f, 0.9f, 1.0f));

		ReloadButton->BackgroundColor = FLinearColor(0.f, 0.5f, 1.f, 1.f);
		Skill3ButtonImage->SetColorAndOpacity(FLinearColor(0.9f, 0.9f, 0.9f, 1.0f));

		CoverButton->BackgroundColor = FLinearColor(0.f, 0.5f, 1.f, 1.f);
		Skill4ButtonImage->SetColorAndOpacity(FLinearColor(0.9f, 0.9f, 0.9f, 1.0f));


		Skill1CooldownMarker->SetVisibility(ESlateVisibility::Visible);
		Skill1CooldownValue->SetVisibility(ESlateVisibility::Visible);

		Skill2CooldownMarker->SetVisibility(ESlateVisibility::Visible);
		Skill2CooldownValue->SetVisibility(ESlateVisibility::Visible);

	}
	else {
		AttackButton->BackgroundColor = FLinearColor(0.1f, 0.1f, 0.1f, 1.f);
		Skill0ButtonImage->SetColorAndOpacity(FLinearColor(0.1f, 0.1f, 0.1f, 1.f));

		Skill1Button->BackgroundColor = FLinearColor(0.1f, 0.1f, 0.1f, 1.f);
		Skill1ButtonImage->SetColorAndOpacity(FLinearColor(0.1f, 0.1f, 0.1f, 1.f));

		Skill2Button->BackgroundColor = FLinearColor(0.1f, 0.1f, 0.1f, 1.f);
		Skill2ButtonImage->SetColorAndOpacity(FLinearColor(0.1f, 0.1f, 0.1f, 1.f));

		ReloadButton->BackgroundColor = FLinearColor(0.1f, 0.1f, 0.1f, 1.f);
		Skill3ButtonImage->SetColorAndOpacity(FLinearColor(0.1f, 0.1f, 0.1f, 1.f));

		CoverButton->BackgroundColor = FLinearColor(0.1f, 0.1f, 0.1f, 1.f);
		Skill4ButtonImage->SetColorAndOpacity(FLinearColor(0.1f, 0.1f, 0.1f, 1.f));

		Skill1CooldownMarker->SetVisibility(ESlateVisibility::Hidden);
		Skill1CooldownValue->SetVisibility(ESlateVisibility::Hidden);

		Skill2CooldownMarker->SetVisibility(ESlateVisibility::Hidden);
		Skill2CooldownValue->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UBattleWidget::ChangeSelectedButton(class UButton* SelectedButton)
{
	if (this->SelectedButton != nullptr) {
		if (this->SelectedButton == GetAttackButton())
			this->SelectedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		else if (this->SelectedButton == GetSkill1Button())
			this->SelectedButton->SetBackgroundColor(FLinearColor(0.0f, 1.0f, 0.f, 1.f));
		else if (this->SelectedButton == GetSkill2Button())
			this->SelectedButton->SetBackgroundColor(FLinearColor(0.f, 1.0f, 0.f, 1.f));
		else if (this->SelectedButton == GetReloadButton())
			this->SelectedButton->SetBackgroundColor(FLinearColor(0.f, 0.5f, 1.f, 1.f));
		else if (this->SelectedButton == GetCoverButton())
			this->SelectedButton->SetBackgroundColor(FLinearColor(0.f, 0.5f, 1.f, 1.f));


		this->SelectedButton = SelectedButton;
		this->SelectedButton->SetBackgroundColor(FColor::Yellow);
		
	}
	else {
		this->SelectedButton = SelectedButton;
		this->SelectedButton->SetBackgroundColor(FColor::Yellow);
	}

}

void UBattleWidget::Init_SkillButtonColor()
{
	GetAttackButton()->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
	GetSkill1Button()->SetBackgroundColor(FLinearColor(0.0f, 1.0f, 0.f, 1.f));
	GetSkill2Button()->SetBackgroundColor(FLinearColor(0.f, 1.0f, 0.f, 1.f));
	GetReloadButton()->SetBackgroundColor(FLinearColor(0.f, 0.5f, 1.f, 1.f));
	GetCoverButton()->SetBackgroundColor(FLinearColor(0.f, 0.5f, 1.f, 1.f));
}

void UBattleWidget::Set_BattleWidgetSkillButtonActive(bool Active)
{
	AttackButton->SetIsEnabled(Active);
	Skill1Button->SetIsEnabled(Active);
	Skill2Button->SetIsEnabled(Active);
	ReloadButton->SetIsEnabled(Active);
	CoverButton->SetIsEnabled(Active);

}
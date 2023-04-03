// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleWidget.generated.h"

/**
 * 
 */
UCLASS()
class SQUAD_API UBattleWidget : public UUserWidget
{
	GENERATED_BODY()

		virtual void NativeConstruct() override;
		

	UFUNCTION()
		void OnClickMoveButton();

	UFUNCTION()
		void OnClickAttackButton();

	UFUNCTION()
		void OnClickCoverButton();

	UFUNCTION()
		void OnClickReloadButton();

	UFUNCTION()
		void OnClickSkill1Button();

	UFUNCTION()
		void OnClickSkill2Button();

	////////////////////////////////////////

	UPROPERTY(meta = (BindWidget))
		class UButton* AttackButton;

	//UPROPERTY(meta = (BindWidget))
	//	class UButton* MoveButton;
	   
	UPROPERTY(meta = (BindWidget))
		class UButton* CoverButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* ReloadButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* Skill1Button;

	UPROPERTY(meta = (BindWidget))
		class UButton* Skill2Button;

	//////////////////////////////////////





	UPROPERTY(meta = (BindWidget))
		class UTextBlock* WhosTurn;


	/////////////////////////////////////////
	
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TargetNameBlock;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TargetCharacterMaxAmmo;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TargetCharacterCurrentAmmo;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TargetCharacterMaxLifePoint;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TargetCharacterCurrentLifePoint;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TargetCharacterLifePointMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TargetCharacterAmmoMarker;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TargetCharacterHPSlash;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TargetCharacterAmmoSlash;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TargetCharacterEvasion;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TargetCharacterDefense;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TargetCharacterAccurancy;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TargetCharacterCritical;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TargetCharacterDamage;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TargetCharacterHit;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TargetCharacterMaxDamage;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TargetCharacterEvasionMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TargetCharacterDefenseMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TargetCharacterAccurancyMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TargetCharacterCriticalMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TargetCharacterDamageMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TargetCharacterHitMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TargetCharacterMaxDamageMarker;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TargetCharacterTotalAccurancy;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TargetCharacterTotalAccurancyMarker;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TargetCharacterTotalCritical;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TargetCharacterTotalCriticalMarker;


	/////////////////////////////////////////

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CurrentCharacter;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterMaxAmmo;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterCurrentAmmo;
	UPROPERTY(meta = (BindWIdget))
		class UTextBlock* SelectedCharacterMaxLifePoint;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterCurrentLifePoint;


	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterAccuracy;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterCritical;
	UPROPERTY(meta = (BindWIdget))
		class UTextBlock* SelectedCharacterDamage;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterHitCount;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterMaxDamage;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterEvasion;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterDefense;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterLifePointMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterAmmoMarker;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterHPSlash;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterAmmoSlash;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterAccuracyMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterCriticalMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterMaxDamageMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterDamageMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterHitCountMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterEvasionMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterDefenseMarker;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterSkillName;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterSkillAccurancy;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterSkillCritical;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterSkillCooldown;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterSkillExplanation;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterSkillAttribute;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterSkillAttributeEffect;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterSkillMarker1;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterSkillMarker2;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterSkillMarker3;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterSkillMarker4;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterSkillMarker5;

	/////////////////////////////////////

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterDMGGridMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterACCGridMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterCritGridMarker;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterDamage0GridMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterDamage1GridMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterDamage2GridMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterDamage3GridMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterDamage4GridMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterDamage5GridMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterDamage6GridMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterDamage7GridMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterDamage8GridMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterDamage9GridMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterDamage10GridMarker;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterDamage0perGridMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterDamage1perGridMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterDamage2perGridMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterDamage3perGridMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterDamage4perGridMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterDamage5perGridMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterDamage6perGridMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterDamage7perGridMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterDamage8perGridMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterDamage9perGridMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterDamage10perGridMarker;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectedCharacterCritperGridMarker;


	/////////////////////////////////////

	UPROPERTY(meta = (BindWidget))
		class UImage* SelectedCharacterSkillGraphWhiteBackground;
	
	UPROPERTY(meta = (BindWidget))
		class UImage* SelectedCharacterSkillGraphBlackBackground;

	UPROPERTY(meta = (BindWidget))
		class UHorizontalBox* SelectedCharacterSkillGraphMarker;

	UPROPERTY(meta = (BindWidget))
		class UCanvasPanel* BattlewidgetRightPanelBackground;
	UPROPERTY(meta = (BindWidget))
		class UCanvasPanel* BattlewidgetLeftPanelBackground;

public:

	void ChangeTargetName(FString NAME);
	void ChangeCurrentCharName(FString NAME);
	void ChangeWhosTurnName(bool NAME);

	void ChangeCurrentAmmoText(float CurrentAmmo);
	void ChangeMaxAmmoText(float MaxAmmo);

	void ChangeSkillButtonImage(class APlayerSquadCharacter * PlayerCharacter);

	void UpdateCharacterSkillMarker(APlayerSquadCharacter * PlayerCharacter);

	void UpdateCharacterInfoText(FText Name, float MaxAmmo, float CurrentAmmo, float MaxLifePoint, float CurrentLifePoint);


	FString InitName = TEXT("NULL");

public:
		
	//void UpdateCharacterInfoText(FString Name, float MaxAmmo, float CurrentAmmo, float MaxLifePoint, float CurrentLifePoint);

	void UpdateWiget_TargetCharacterPanel(class ASquadCharacter * TargetChar);
	void UpdateWiget_TargetCharacterStatPanel(class ASquadCharacter* TargetChar, class ASquadCharacter* SelectedChar);

	//void UpdateCharacterInfoText_Right(FString Name, float MaxAmmo, float CurrentAmmo, float MaxLifePoint, float CurrentLifePoint, class ASquadCharacter* TargetChar);
	void UpdateCharacterInfoText_Right2(float Acc, float Crit, float Damage, float Hitcount, float MaxDamage, float Evasion, float Defense , class ASquadCharacter* Char);

	void UpdateCharacterDamageGraph(ASquadCharacter * Char);

	void UpdateCharacterDamageGraph_Blank();
	

	void Updata_CharacterSkill_Info(class APlayerSquadCharacter * Char, int32 skillNumber);

	void ClearCharacterInfoText_Left();
	void ClearCharacterInfoText_Right();

	UFUNCTION()
	void SetDamageMarkerColor(UTextBlock* DamageText , float per, float maxPer, float per_float);

	void Set_BattleWidgetOpacity(float val);

	void UpdateWidget_SkillPart(class APlayerSquadCharacter * Pcharacter, float SkillNum);

	void ClearWidget_SkillPart();

	void Set_BattleWidgetSkilliconOpacity(bool Active);


	UPROPERTY(meta = (BindWidget))
		class UImage* Skill0ButtonImage;
	UPROPERTY(meta = (BindWidget))
		class UImage* Skill1ButtonImage;
	UPROPERTY(meta = (BindWidget))
		class UImage* Skill2ButtonImage;
	UPROPERTY(meta = (BindWidget))
		class UImage* Skill3ButtonImage;
	UPROPERTY(meta = (BindWidget))
		class UImage* Skill4ButtonImage;

	UPROPERTY(meta = (BindWidget))
		class UImage* SelectedCharacterBattleTable;

	class UTexture2D* Skill1ButtonTexture2D;
	class UTexture2D* Skill2ButtonTexture2D;

	class UButton* SelectedButton = nullptr;
	void ChangeSelectedButton(class UButton* SelectedButton);

	void Init_SkillButtonColor();

	void Set_BattleWidgetSkillButtonActive(bool Active);
	class UButton* GetAttackButton() { return AttackButton; };
	class UButton* GetSkill1Button() { return Skill1Button; };
	class UButton* GetSkill2Button() { return Skill2Button; };
	class UButton* GetCoverButton() { return CoverButton; };
	class UButton* GetReloadButton() { return ReloadButton; };

	UPROPERTY(meta = (BindWidget))
		class UBorder* Skill1CooldownMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Skill1CooldownValue;
	UPROPERTY(meta = (BindWidget))
		class UBorder* Skill2CooldownMarker;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Skill2CooldownValue;

};

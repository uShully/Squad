// Fill out your copyright notice in the Description page of Project Settings.

#include "SquadCharacterSkillComponent.h"
#include "PlayerSquadCharacter.h"
#include "Engine/Engine.h"
#include "SquadCharacterStatComponent.h"
#include "SquadGameInstance.h"
#include "Grid.h"
#include "GridManager.h"

// Sets default values for this component's properties
USquadCharacterSkillComponent::USquadCharacterSkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;



	// ...
}


// Called when the game starts
void USquadCharacterSkillComponent::BeginPlay()
{
	Super::BeginPlay();

}

void USquadCharacterSkillComponent::InitCharacterSkillData()
{
	// DT에서 스킬 데이터 호출 및 초기화
	auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

	FSkillValueList* skill0 = gameIns->GetSkillValueData(0); // 사격
	FSkillValueList* skill1;
	FSkillValueList* skill2;
	FSkillValueList* skill3 = gameIns->GetSkillValueData(13); // 엄폐
	FSkillValueList* skill4 = gameIns->GetSkillValueData(14); // 대기

	// 각 병과의 스킬 데이터 호출
	// 0.10.14 - 다음 업데이트(병과 추가)시 변경 지점
	if (pOwner->ClassNum == 0) {
		skill1 = gameIns->GetSkillValueData(1);
		skill2 = gameIns->GetSkillValueData(2);

	}
	else if (pOwner->ClassNum == 3) {
		skill1 = gameIns->GetSkillValueData(4);
		skill2 = gameIns->GetSkillValueData(5);
	}
	else if (pOwner->ClassNum == 5) {

		skill1 = gameIns->GetSkillValueData(7);
		skill2 = gameIns->GetSkillValueData(8);
	}
	else if (pOwner->ClassNum == 8) {
		skill1 = gameIns->GetSkillValueData(10);
		skill2 = gameIns->GetSkillValueData(12);		
	}

	// 스킬 데이터 배열에 저장
	SkillArray.Add(skill0);
	SkillArray.Add(skill1);
	SkillArray.Add(skill2);
	SkillArray.Add(skill3);
	SkillArray.Add(skill4);	
}

FSkillValueList* USquadCharacterSkillComponent::Get_Skill0Data()
{
	return SkillArray[0];
}

FSkillValueList* USquadCharacterSkillComponent::Get_Skill1Data()
{
	return SkillArray[1];
}

FSkillValueList* USquadCharacterSkillComponent::Get_Skill2Data()
{
	return SkillArray[2];
}

FSkillValueList* USquadCharacterSkillComponent::Get_Skill3Data()
{
	return SkillArray[3];
}

FSkillValueList* USquadCharacterSkillComponent::Get_Skill4Data()
{
	return SkillArray[4];
}

void USquadCharacterSkillComponent::UseCharacterSkill(int32 skillNum, AActor* TargetCharacter)
{
	Get_SkillDataForUse(skillNum , TargetCharacter);

}

void USquadCharacterSkillComponent::InitCharacterSkillComp(int32 classNum, AActor* Owner)
{
	this->classNum = classNum;
	pOwner = Cast<APlayerSquadCharacter>(Owner);
	InitCharacterSkillData();
}

void USquadCharacterSkillComponent::Calc_SkillData(AActor* Target, int32 SkillNumber)
{
	ASquadCharacter* tempTarget = Cast<ASquadCharacter>(Target);	


	pOwner->Calc_SkillDamage_distribution(tempTarget, SkillArray[SkillNumber]);

}

void USquadCharacterSkillComponent::Get_SkillDataForUse(int32 skillNum , AActor* Target)
{	

	ASquadCharacter* tempTarget = Cast<ASquadCharacter>(Target);
		
	Skill_DamageCalc(skillNum, Target);
	Skill_Specialability(skillNum, Target);

	if (pOwner->CurrentAmmo >= 0)
		pOwner->CurrentAmmo -= 1;

	if (skillNum == 1)
		pOwner->Character_Skill1_Cooldown = SkillArray[1]->CooldownValue;
	else if (skillNum == 2)
		pOwner->Character_Skill2_Cooldown = SkillArray[2]->CooldownValue;
	

	pOwner->SetTurnOffHighLightGrid();

}

float USquadCharacterSkillComponent::Skill_DamageCalc(int32 skillNum, AActor* Target)
{
	ASquadCharacter* tempTarget = Cast<ASquadCharacter>(Target);

	int32 HitCount = 0;
	int32 CriticalCount = 0;
	float ActualDamage = 0.f;

	int32 ShotCount = SkillArray[skillNum]->SkillFireCount; // 여러횟수 발사시 사용예정
	int32 bulletCount = SkillArray[skillNum]->SkillBulletCount;
	int32 bulletDamage = SkillArray[skillNum]->SkillDamage;
	int32 AccuracyCorrections = SkillArray[skillNum]->Special_ability_1_UsedCharacterAccurancyrateCorrectionValue;
	int32 CriticalCorrections = SkillArray[skillNum]->Special_ability_2_UsedCharacterCriticalCorrectionValue;
	int32 DefenseValue = tempTarget->Defense;

	for (int32 i = 0; i < bulletCount; i++)
	{
		if (FMath::FRandRange(0.f, 100.f) < pOwner->CharacterStat->GetCharacterAccuracyCorrectionValue() + pOwner->CharacterStat->GetWeaponAccuracy() - tempTarget->Evasion + (AccuracyCorrections))
		{
			HitCount++; // 히트수 계산
		}
	}
	for (int32 i = 0; i < HitCount; i++)
	{
		if (FMath::FRandRange(0.f, 100.f) < pOwner->CharacterStat->GetCharacterCriticalCorrectionValue() + pOwner->CharacterStat->GetWeaponCritical() + CriticalCorrections - DefenseValue)
		{
			CriticalCount++; // 히트수를 가지고 치명타 계산
		}
	}

	ActualDamage = bulletDamage * HitCount + 1000.f * CriticalCount;

	if (ActualDamage > 0)
		UGameplayStatics::ApplyDamage(tempTarget, ActualDamage, GetWorld()->GetFirstPlayerController(), pOwner, nullptr);
	else if (ActualDamage <= 0) {
		FDamageEvent DamegeEvent;
		Cast<ASquadCharacter>(tempTarget)->TakeDamage(0, DamegeEvent, nullptr, pOwner);
	}
		//UGameplayStatics::ApplyDamage(tempTarget, 0, GetWorld()->GetFirstPlayerController(), pOwner, nullptr);

	return ActualDamage;
}

void USquadCharacterSkillComponent::Skill_Specialability(int32 skillNum, AActor* Target)
{
	ASquadCharacter* tempTarget = Cast<ASquadCharacter>(Target);
	FCCStatus TargetCCStatus;

	if (SkillArray[skillNum]->Special_ability_3_TargetAvoidancerateCorrectionValue != 0) {
		TargetCCStatus.Skill_TargetAvoidanceRateCorrectionValue = SkillArray[skillNum]->Special_ability_3_TargetAvoidancerateCorrectionValue;
		TargetCCStatus.Skill_Count = 1;
		TargetCCStatus.SetUseCharacterData(pOwner);
		TargetCCStatus.SetTargetCharacterData(Cast<ASquadCharacter>(Target));
		tempTarget->Add_CCArray(TargetCCStatus);
		TargetCCStatus.ClearStructData();
	}
	if (SkillArray[skillNum]->Special_ability_4_TargetAccurancyrateCorrectionValue != 0) {
		TargetCCStatus.Skill_TargetAccurancyRateCorretionValue = SkillArray[skillNum]->Special_ability_4_TargetAccurancyrateCorrectionValue;
		TargetCCStatus.Skill_Count = 1;
		TargetCCStatus.SetUseCharacterData(pOwner);
		TargetCCStatus.SetTargetCharacterData(Cast<ASquadCharacter>(Target));
		tempTarget->Add_CCArray(TargetCCStatus);
		TargetCCStatus.ClearStructData();
	}
	if (SkillArray[skillNum]->Special_ability_8_Stun_available) {
		TargetCCStatus.Skill_Stun = SkillArray[skillNum]->Special_ability_8_Stun_available;
		TargetCCStatus.Skill_Count = SkillArray[skillNum]->Special_ability_9_stun_SustainmentTime;
		TargetCCStatus.SetUseCharacterData(pOwner);
		TargetCCStatus.SetTargetCharacterData(Cast<ASquadCharacter>(Target));
		tempTarget->Add_CCArray(TargetCCStatus);
		TargetCCStatus.ClearStructData();		
	}
	if (SkillArray[skillNum]->Special_ability_6_RangAttack_TopBottomColum == 1) {
		TArray<AActor*> RangeTargetArray = RangeAttackSkill_AdjacentTarget(Target);
		if(RangeTargetArray.Num() > 0) {
			for(int8 i = 0 ; i < RangeTargetArray.Num() ; i++)
				Skill_DamageCalc(skillNum, RangeTargetArray[i]);
		}
	}

	if (SkillArray[skillNum]->SkillAttribute == TEXT("Normally")) {
		TargetCCStatus.Skill_UsedCharacterEvasionCorrectionValue = 0;
		TargetCCStatus.Skill_UsedCharacterDefensiveCorrectionValue = 0;
		TargetCCStatus.Skill_Count = 1;

		TargetCCStatus.SetUseCharacterData(pOwner);
		TargetCCStatus.SetTargetCharacterData(pOwner);
		pOwner->Add_CCArray(TargetCCStatus);
		TargetCCStatus.ClearStructData();
	}
	else if(SkillArray[skillNum]->SkillAttribute == TEXT("Offensive")) {
		TargetCCStatus.Skill_UsedCharacterEvasionCorrectionValue = -10;
		TargetCCStatus.Skill_UsedCharacterDefensiveCorrectionValue = -5;
		TargetCCStatus.Skill_Count = 1;

		TargetCCStatus.SetUseCharacterData(pOwner);
		TargetCCStatus.SetTargetCharacterData(pOwner);
		pOwner->Add_CCArray(TargetCCStatus);
		TargetCCStatus.ClearStructData();
	}
	else if (SkillArray[skillNum]->SkillAttribute == TEXT("Defensive")) {
		TargetCCStatus.Skill_UsedCharacterEvasionCorrectionValue = 10;
		TargetCCStatus.Skill_UsedCharacterDefensiveCorrectionValue = 5;
		TargetCCStatus.Skill_Count = 1;

		TargetCCStatus.SetUseCharacterData(pOwner);
		TargetCCStatus.SetTargetCharacterData(pOwner);
		pOwner->Add_CCArray(TargetCCStatus);
		TargetCCStatus.ClearStructData();
	}

}

TArray<AActor*> USquadCharacterSkillComponent::RangeAttackSkill_AdjacentTarget(AActor* targetCharacter)
{
	ASquadCharacter* target = Cast<ASquadCharacter>(targetCharacter);
	AGridManager* parentEB = target->GetUnderGrid()->parentGridManager;
	TArray<AActor*> tempRangeTargetArray;
	TArray<AGrid*> tempGridArray;

	int32 Xpos = target->GetUnderGrid()->XPos;
	int32 Ypos = target->GetUnderGrid()->YPos;
	
	auto GridArray = parentEB->XGridArray;

	//for(int32 i = 0; GridArray.Num() ; i++) {
	
	for (int32 i = 0; i < GridArray.Num(); i++) {
		for (int32 j = 0; j < GridArray[i].Num(); j++) {
			if (GridArray[i][j]->GridInfo.GOTO == EGridOntheObject::Enemy) {
				if (Ypos - 1 <= j && Ypos + 1 >= j)
					tempGridArray.Add(GridArray[i][j]);				
			}
		}
	}
	
	for(int32 i = 0 ; i < tempGridArray.Num() ; i++) {
		if (Cast<AEnemySquadCharacter>(tempGridArray[i]->GridInfo.GridOnTheCharacter)->StateEnum != EStateEnum::SE_Death && Cast<AEnemySquadCharacter>(tempGridArray[i]->GridInfo.GridOnTheCharacter) != targetCharacter) {
			tempRangeTargetArray.Add(Cast<AEnemySquadCharacter>(tempGridArray[i]->GridInfo.GridOnTheCharacter));
		}
	}
	
	return tempRangeTargetArray;
}
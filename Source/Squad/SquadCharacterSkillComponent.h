// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SquadCharacterSkillComponent.generated.h"

UENUM(BlueprintType)
enum class EClassEnum : uint8
{            											     
	RifleMan UMETA(DisplayName = "RifleMan"),
	Dominator UMETA(DisplayName = "Dominator"),
	_NULL_	UMETA(DisplayName = "_NULL_"),
	Police UMETA(DisplayName = "Police"),
	Medic UMETA(DisplayName = "Medic"),
	Assault UMETA(DisplayName = "Assault"),
	Ranger UMETA(DisplayName = "Ranger"),
	Sniper UMETA(DisplayName = "Sniper"),
	Scout UMETA(DisplayName = "Scout"),
};

USTRUCT(BlueprintType)
struct FPlayerSkillData
{
	GENERATED_BODY()

public:		
	
	FPlayerSkillData() {}

	FPlayerSkillData(int32 InSkill_ShotCount, int32 InSkill_bulletCount, int32 InSkill_bulletDamage, int32 InSkill_AccuracyCorrections)
		: Skill_ShotCount(InSkill_ShotCount), Skill_bulletCount(InSkill_bulletCount), Skill_bulletDamage(InSkill_bulletDamage), Skill_AccuracyCorrections(InSkill_AccuracyCorrections)
	{}

	int32 GetSkill_ShotCount() { return Skill_ShotCount; };
	int32 GetSkill_bulletCount() { return Skill_bulletCount; };
	int32 GetSkill_bulletDamage() { return Skill_bulletDamage; };
	int32 GetSkill_AccuracyCorrections() { return Skill_AccuracyCorrections; };

private:
	 
	UPROPERTY()
		int32 Skill_ShotCount = 0;
	UPROPERTY()
		int32 Skill_bulletCount = 0;
	UPROPERTY()
		int32 Skill_bulletDamage = 0;
	UPROPERTY()
		int32 Skill_AccuracyCorrections = 0;



};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SQUAD_API USquadCharacterSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USquadCharacterSkillComponent();
	void InitCharacterSkill();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	

public:	
	
	void UseCharacterSkill(int32 skillNum, AActor* TargetCharacter);
	void SetCharacterData(int32 classNum, AActor* Owner);

	int32 GetSkill1Cooldown() { return Skill1Cooldown; };
	int32 GetSkill2Cooldown() { return Skill2Cooldown; };	

	FPlayerSkillData* GetSkill1Data() { return Skill1Data; };

	void Calc_SkillData(AActor* Target, int32 SkillNumber);

private:

	class APlayerSquadCharacter* pOwner;
	int32 classNum = 0;
	int32 Skill1Cooldown = 0;
	int32 Skill2Cooldown = 0;



	void Get_SkillDataForUse(int32 skillNum, AActor * Target);

	float Skill_DamageCalc(int32 skillNum, AActor * Target);

	void Skill_Specialability(int32 skillNum, AActor * Target);

	TArray<AActor*> RangeAttackSkill_AdjacentTarget(AActor * targetCharacter);


		
	
	FPlayerSkillData *Skill1Data;
	FPlayerSkillData *Skill2Data;

	float Skill_ActualDamage = 0.f;

	TMap<int32, FPlayerSkillData> SkillDataMap;

private:
	TArray<struct FSkillValueList*> SkillArray;

public:
	FSkillValueList* Get_Skill0Data();
	FSkillValueList* Get_Skill1Data();
	FSkillValueList* Get_Skill2Data();
	FSkillValueList * Get_Skill3Data();
	FSkillValueList * Get_Skill4Data();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "BattleController.h"
#include "SquadCharacter.h"
#include "PlayerSquadCharacter.h"
#include "EnemySquadCharacter.h"
#include "SquadCameraManager.h"
#include "CommonStruct.h"
#include "SquadGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FSquadCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FSquadCharacterData() : Level(1), MaxHP(100.f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Data")
	float MaxHP;

};

USTRUCT(Atomic, BlueprintType)
struct FPlayerCharacterSlot
{
	GENERATED_BODY()

public:
	//APlayerSquadCharacter* CharacterInfo;
	TArray<FCharacterDataStruct> CharacterDataArry;
	int32 SlotNum;

	int32 CalSlotNum()
	{
		SlotNum = CharacterDataArry.Num();
	
		return SlotNum;
	}

	void EmptyCharacterDataArry()
	{
		CharacterDataArry.Empty();
		SlotNum = 0;
	}

	int32 MaxSlotNum = 4;
};

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

public:

	FWeaponData() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float WeaponNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString WeaponName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString ContentPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float WeaponMaxFireCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float WeaponDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float WeaponFireCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float WeaponAccuracy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float WeaponCritical;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float WeaponAvoidence;



};

USTRUCT(BlueprintType)
struct FBrunchData : public FTableRowBase
{
	GENERATED_BODY()

public:

	FBrunchData() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float BrunchNumber; // ClassNum
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float WeaponNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString EquipWeaponName; // 캐릭터 선택 기획 재정립 후 다시 수정 ->FString EquipWeapon	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float AccuracyCorrectionValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float CriticalCorrectionValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float EvasionCorrectionValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float HPValue;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		//FString BrunchName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FText BrunchName = NSLOCTEXT("CharacterBrunch", "Brunch", "BrunchName");

};

USTRUCT(BlueprintType)
struct FSkillValueList : public FTableRowBase
{
	GENERATED_BODY()

public:

	FSkillValueList() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float SkillNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString WeaponName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString BrunchName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString SkillName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString SkillAttribute;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString SkillAttributeExplanation;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float SkillFireCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float SkillBulletCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float SkillDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float SkillAvoidancerateCorrectionValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float DefenseValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float CooldownValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Special_ability_1_UsedCharacterAccurancyrateCorrectionValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Special_ability_2_UsedCharacterCriticalCorrectionValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Special_ability_3_TargetAvoidancerateCorrectionValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Special_ability_4_TargetAccurancyrateCorrectionValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Special_ability_5_SelfHealing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Special_ability_6_RangAttack_TopBottomColum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Special_ability_7_RangeAttack_All;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		bool Special_ability_8_Stun_available;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Special_ability_9_stun_SustainmentTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString Skill_icon_DataPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString SkillExplanation;

};

USTRUCT(BlueprintType)
struct FEnemyBrunchData : public FTableRowBase
{
	GENERATED_BODY()

public:

	FEnemyBrunchData() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float BrunchNumber; // ClassNum
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString BrunchName; // ClassNum
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString EquipWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float AccuracyCorrectionValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float CriticalCorrectionValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float EvasionCorrectionValue;



};

USTRUCT(BlueprintType)
struct FEventValue : public FTableRowBase
{
	GENERATED_BODY()

public:

	FEventValue() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 mainStreamNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		EDecisionEnum EventType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float FirstOptionProbability;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		EDecisionEventResultEnum FirstOptionSuccResult;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 FirstOptionSuccResultValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		EDecisionEventResultEnum FirstOptionFailResult;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 FirstOptionFailResultValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float SecondOptionProbability;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		EDecisionEventResultEnum SecondOptionSuccResult;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 SecondOptionSuccResultValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		EDecisionEventResultEnum SecondOptionFailResult;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 SecondOptionFailResultValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float ThirdOptionProbability;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		EDecisionEventResultEnum ThirdOptionSuccResult;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 ThirdOptionSuccResultValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		EDecisionEventResultEnum ThirdOptionFailResult;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 ThirdOptionFailResultValue;




};

USTRUCT(BlueprintType)
struct FEventProb : public FTableRowBase
{
	GENERATED_BODY()

public:

	FEventProb() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 EventNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float EventProb;


};



USTRUCT(BlueprintType)
struct FEventDirectionData : public FTableRowBase
{
	GENERATED_BODY()

public:

	FEventDirectionData() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString EventNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString AnimationPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString SoundPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString FXPath;

};
/**
 * 
 */
UCLASS()
class SQUAD_API USquadGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	USquadGameInstance();

	virtual void Init() override;

	//virtual void BeginPlay() override;

	void IncludeCharacterData(FCharacterDataStruct CharacterData);
	void ExcludeCharacterData();
	void ExcludeDeadCharacterData(APlayerSquadCharacter * DeadCharacter);
	int32 GetCharacterDataNum();



	FPlayerCharacterSlot CharSlot;

	void CharSlotEmpty();

	//	UPROPERTY()
		//TArray<FPlayerCharacterSlot> CharSlot;

		///////////////

	ABattleController* BCIns;
	ASquadCameraManager* SCMIns;


	AActor* TargetCharacter;
	AActor* SelectedCharacter;

	UFUNCTION()
		void InitInstance();



	FSquadCharacterData* GetSquadCharacterData(int32 weaponNumber);

public:

	bool IsBattleStart = false;


private:
	UPROPERTY()
		class UDataTable* SquadCharacterTable;

public:
	FWeaponData* GetWeaponData(int32 WeaponNum);
	FBrunchData* GetBrunchData(int32 BrunchNum);
	FSkillValueList* GetSkillValueData(int32 SkillNum);
	FEventValue* GetEventValueData(int32 mainEventStream);

	FEventProb* GetBattleEventProbData(int32 EventNum);
	FEventProb* GetNonBattleEventProbData(int32 EventNum);

	FEnemyBrunchData* GetEnemyBrunchData(int32 BrunchNum);

	UPROPERTY(BlueprintReadWrite)
		float Volume_Master = 0.5f;
	UPROPERTY(BlueprintReadWrite)
		float Volume_BGM = 0.5f;
	UPROPERTY(BlueprintReadWrite)
		float Volume_SFX = 0.5f;
	UPROPERTY(BlueprintReadWrite)
		float Volume_Voice = 0.5f;


	FIteminventory* GetPlayerInventory() { return &PlayerInventory; };

private:
	UPROPERTY()
		class UDataTable* WeaponDataTable;

	UPROPERTY()
		class UDataTable* BrunchDataTable;

	UPROPERTY()
		class UDataTable* SkillValueTable;

	UPROPERTY()
		class UDataTable* EventValueTable;

	UPROPERTY()
		class UDataTable* BattleEventProbTable;

	UPROPERTY()
		class UDataTable* NonBattleEventProbTable;

	UPROPERTY()
		struct FIteminventory PlayerInventory;

	UPROPERTY()
		class UDataTable* EnemyBrunchDataTable;

public:
	UPROPERTY()
		TArray<int32> Mission1_mapData = { 1 , 2, 1, 2, 3};

	UPROPERTY()
		int32 GameDifficulty = 0;

	UPROPERTY()
		int32 InitialGameDifficulty = 0;
};


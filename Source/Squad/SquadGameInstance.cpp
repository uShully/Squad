// Fill out your copyright notice in the Description page of Project Settings.

#include "SquadGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "BattleTrigger.h"


// 데이터 저장용 인스턴스


USquadGameInstance::USquadGameInstance()
{
	
	FString WeaponDataPath = L"DataTable'/Game/DATATABLE/AWeaponData.AWeaponData'";
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_WEAPON(*WeaponDataPath);
	WeaponDataTable = DT_WEAPON.Object;

	FString BrunchDataPath = L"DataTable'/Game/DATATABLE/ABrunchData.ABrunchData'";
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Brunch(*BrunchDataPath);
	BrunchDataTable = DT_Brunch.Object;
	
	FString SkillValueListPath = L"DataTable'/Game/DATATABLE/ASkillValueList.ASkillValueList'";
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Skill(*SkillValueListPath);
	SkillValueTable = DT_Skill.Object;

	FString EventValueListPath = L"DataTable'/Game/DATATABLE/AEventValue.AEventValue'";
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Event(*EventValueListPath);
	EventValueTable = DT_Event.Object;

	FString BattleEventProbListPath = L"DataTable'/Game/DATATABLE/BattleEventProbData.BattleEventProbData'";
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_BattleEventProb(*BattleEventProbListPath);
	BattleEventProbTable = DT_BattleEventProb.Object;

	FString NonBattleEventProbListPath = L"DataTable'/Game/DATATABLE/NonBattleEventProbData.NonBattleEventProbData'";
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_NonBattleEventProb(*NonBattleEventProbListPath);
	NonBattleEventProbTable = DT_NonBattleEventProb.Object;

	FString EnemyBrunchDataPath = L"DataTable'/Game/DATATABLE/AEnemyBrunchData.AEnemyBrunchData'";
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_EnemyBrunch(*EnemyBrunchDataPath);
	EnemyBrunchDataTable = DT_EnemyBrunch.Object;
}

void USquadGameInstance::Init()
{
	Super::Init();
	// Init -> PostInitializeComponent -> PostLogin -> StartPlay or BeginPlay 순

	//UE_LOG(LogClass, Log, L" MaxHP of Level 2 SquadCharacter : %f", GetSquadCharacterData(2)->MaxHP);
}

FSquadCharacterData* USquadGameInstance::GetSquadCharacterData(int32 weaponNumber)
{
	return SquadCharacterTable->FindRow<FSquadCharacterData>(*FString::FromInt(weaponNumber), TEXT(""));
}

void USquadGameInstance::InitInstance()
{
	BCIns = nullptr;
	SCMIns = nullptr;


	TargetCharacter = nullptr;
	SelectedCharacter = nullptr;
}

void USquadGameInstance::IncludeCharacterData(FCharacterDataStruct CharacterData)
{
	CharSlot.CharacterDataArry.Add(CharacterData);
	CharSlot.CalSlotNum();
}

void USquadGameInstance::ExcludeCharacterData()
{
	CharSlot.CharacterDataArry.Pop();
	CharSlot.CalSlotNum();
}

int32 USquadGameInstance::GetCharSlotNum()
{
	return  CharSlot.SlotNum;
}

FWeaponData* USquadGameInstance::GetWeaponData(int32 WeaponNum)
{
	return WeaponDataTable->FindRow<FWeaponData>(*FString::FromInt(WeaponNum), TEXT(""));
	// ex) GetWeaponData(0)->Damage
}

FBrunchData* USquadGameInstance::GetBrunchData(int32 BrunchNum)
{
	return BrunchDataTable->FindRow<FBrunchData>(*FString::FromInt(BrunchNum), TEXT(""));
}

FSkillValueList* USquadGameInstance::GetSkillValueData(int32 SkillNum)
{
	return SkillValueTable->FindRow<FSkillValueList>(*FString::FromInt(SkillNum), TEXT(""));
}

FEventValue* USquadGameInstance::GetEventValueData(int32 mainEventStream)
{
	
	return EventValueTable->FindRow<FEventValue>(*FString::FromInt(mainEventStream), TEXT(""));
}

FEventProb* USquadGameInstance::GetBattleEventProbData(int32 EventNum)
{
	return BattleEventProbTable->FindRow<FEventProb>(*FString::FromInt(EventNum), TEXT(""));
}

FEventProb* USquadGameInstance::GetNonBattleEventProbData(int32 EventNum)
{
	return NonBattleEventProbTable->FindRow<FEventProb>(*FString::FromInt(EventNum), TEXT(""));
}

FEnemyBrunchData* USquadGameInstance::GetEnemyBrunchData(int32 BrunchNum)
{
	return EnemyBrunchDataTable->FindRow<FEnemyBrunchData>(*FString::FromInt(BrunchNum), TEXT(""));
}
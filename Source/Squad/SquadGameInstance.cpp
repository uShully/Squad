// Fill out your copyright notice in the Description page of Project Settings.

#include "SquadGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"


// 데이터 저장용 인스턴스


USquadGameInstance::USquadGameInstance()
{
	/*
	FString CharacterDataPath = L"DataTable'/Game/GameData/ASuqadCharacterData.ASuqadCharacterData'";
	
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_SQUADCHARACTER(*CharacterDataPath);
	SquadCharacterTable = DT_SQUADCHARACTER.Object;
	*/
}

void USquadGameInstance::Init()
{
	Super::Init();
	// Init -> PostInitializeComponent -> PostLogin -> StartPlay or BeginPlay 순

	//UE_LOG(LogClass, Log, L" MaxHP of Level 2 SquadCharacter : %f", GetSquadCharacterData(2)->MaxHP);
}

FSquadCharacterData* USquadGameInstance::GetSquadCharacterData(int32 Level)
{
	return SquadCharacterTable->FindRow<FSquadCharacterData>(*FString::FromInt(Level), TEXT(""));
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

int32 USquadGameInstance::GetCharacterDataNum()
{
	return  CharSlot.SlotNum;
}


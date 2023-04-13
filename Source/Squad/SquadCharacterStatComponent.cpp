// Fill out your copyright notice in the Description page of Project Settings.

#include "SquadCharacterStatComponent.h"
#include "SquadGameInstance.h"
#include "PlayerSquadCharacter.h"
#include "CommonStruct.h"
#include "Components/SkeletalMeshComponent.h"
#include "CharacterAnimInstance.h"
#include "Engine/Engine.h"


// Sets default values for this component's properties
USquadCharacterStatComponent::USquadCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	static ConstructorHelpers::FClassFinder<UAnimInstance> RIFLE_ANIM(
		TEXT("AnimBlueprint'/Game/ANIMATION/AnimBP/PC_Rifle.PC_Rifle_C'"));
	if (RIFLE_ANIM.Succeeded())
	{
		RifleAnimBP = RIFLE_ANIM.Class;
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> PISTOL_ANIM(
		TEXT("AnimBlueprint'/Game/ANIMATION/AnimBP/PC_Pistol.PC_Pistol_C'"));
	if (PISTOL_ANIM.Succeeded())
	{
		PistolAnimBP = PISTOL_ANIM.Class;
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> SHOTGUN_ANIM(
		TEXT("AnimBlueprint'/Game/ANIMATION/AnimBP/PC_Shotgun.PC_Shotgun_C'"));
	if (SHOTGUN_ANIM.Succeeded())
	{
		ShotgunAnimBP = SHOTGUN_ANIM.Class;
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> SNIPER_ANIM(
		TEXT("AnimBlueprint'/Game/ANIMATION/AnimBP/PC_Sniper.PC_Sniper_C'"));
	if (SNIPER_ANIM.Succeeded())
	{
		SniperAnimBP = SNIPER_ANIM.Class;
	}
	WeaponNum = 0;
	// ...
}


void USquadCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void USquadCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

}


void USquadCharacterStatComponent::SetClassData(int32 ClassNum)
{
	auto SquadGameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

	CurrentBrunchStatData = SquadGameIns->GetBrunchData(ClassNum);
	if (nullptr != CurrentBrunchStatData)
	{
		Character_BrunchNumber = CurrentBrunchStatData->BrunchNumber;
		Character_EquipWeaponNumber = CurrentBrunchStatData->WeaponNumber;
		Character_EquipWeaponName = CurrentBrunchStatData->EquipWeaponName;
		Character_AccuracyCorrectionValue = CurrentBrunchStatData->AccuracyCorrectionValue;
		Character_CriticalCorrectionValue = CurrentBrunchStatData->CriticalCorrectionValue;
		Character_EvasionCorrectionValue = CurrentBrunchStatData->EvasionCorrectionValue;
		Character_HPValue = CurrentBrunchStatData->HPValue;
		auto FBN = FText::FromStringTable("/Game/DATATABLE/BrunchNameData.BrunchNameData", FString::Printf(TEXT("%d"), Character_BrunchNumber));
		Character_BrunchName = FBN; //CurrentBrunchStatData->BrunchName;
	}



	SetWeaponData(Character_EquipWeaponNumber);
	auto Actor = Cast<APlayerSquadCharacter>(GetOwner());
	Actor->InitCharacterStat();
	Cast<UCharacterAnimInstance>(Actor->animInstance)->SetWeaponNumber();
	Cast<UCharacterAnimInstance>(Actor->animInstance)->SetAnimWeaponNumber();
}

void USquadCharacterStatComponent::SetWeaponData(int32 WeaponNumber)
{
	auto SquadGameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

	CurrentStatData = SquadGameIns->GetWeaponData(WeaponNumber);
	if (nullptr != CurrentStatData)
	{
		WeaponContentPath = CurrentStatData->ContentPath;		
		WeaponMaxFireCount = CurrentStatData->WeaponMaxFireCount;	
		WeaponDamage = CurrentStatData->WeaponDamage;		
		WeaponFireCount = CurrentStatData->WeaponFireCount;
		WeaponAccuracy = CurrentStatData->WeaponAccuracy;
		WeaponCritical = CurrentStatData->WeaponCritical;
		WeaponAvoidence = CurrentStatData->WeaponAvoidence;
		auto Actor = Cast<APlayerSquadCharacter>(GetOwner());
		Actor->SetContentMesh(Actor->WeaponSlot, *WeaponContentPath);
		
		if (Character_EquipWeaponName == "Rifle") {
			Actor->GetMesh()->SetAnimInstanceClass(RifleAnimBP);
			Actor->GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			Actor->animInstance = Actor->GetMesh()->GetAnimInstance();
			Actor->ProjectileBulletClass = Actor->AssaultBulletClass;
		}
		else if (Character_EquipWeaponName == "Pistol") {
			Actor->GetMesh()->SetAnimInstanceClass(PistolAnimBP);
			Actor->GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			Actor->animInstance = Actor->GetMesh()->GetAnimInstance();	
			Actor->ProjectileBulletClass = Actor->AssaultBulletClass;
		}
		if (Character_EquipWeaponName == "Shotgun") {
			Actor->GetMesh()->SetAnimInstanceClass(ShotgunAnimBP);
			Actor->GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			Actor->animInstance = Actor->GetMesh()->GetAnimInstance();
			Actor->ProjectileBulletClass = Actor->ShotgunBulletClass;
		}
		else if (Character_EquipWeaponName == "Sniper") {
			Actor->GetMesh()->SetAnimInstanceClass(SniperAnimBP);
			Actor->GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			Actor->animInstance = Actor->GetMesh()->GetAnimInstance();
			Actor->ProjectileBulletClass = Actor->SniperBulletClass;
		}
		
	}
	else
	{
		UE_LOG(LogClass, Log, TEXT(" A : not"));
	}
}

void USquadCharacterStatComponent::SetBranchStat()
{
	auto Actor = Cast<APlayerSquadCharacter>(GetOwner());
	auto SquadGameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

	int32 ClassNumber = Actor->ClassNum;

	CurrentBrunchStatData = SquadGameIns->GetBrunchData(ClassNumber);
}

////////////////////////////////////////////////////////////

float USquadCharacterStatComponent::GetCharacterHPValue() { return Character_HPValue; }
float USquadCharacterStatComponent::GetCharacterAccuracyCorrectionValue() { return Character_AccuracyCorrectionValue; }
float USquadCharacterStatComponent::GetCharacterCriticalCorrectionValue() { return Character_CriticalCorrectionValue; }
float USquadCharacterStatComponent::GetCharacterEvasionCorrectionValue() { return Character_EvasionCorrectionValue; }
FText USquadCharacterStatComponent::GetCharacterBrunchName() { return Character_BrunchName; }


////////////////////////////////////////////////////////////

float USquadCharacterStatComponent::GetWeaponMaxFireCount()
{
	return WeaponMaxFireCount;
}

float USquadCharacterStatComponent::GetWeaponDamage()
{
	return WeaponDamage;
}

float USquadCharacterStatComponent::GetWeaponFireCount()
{
	return WeaponFireCount;
}

float USquadCharacterStatComponent::GetWeaponAccuracy()
{
	return WeaponAccuracy;
}

float USquadCharacterStatComponent::GetWeaponCritical()
{
	return WeaponCritical;
}

float USquadCharacterStatComponent::GetWeaponAvoidence()
{
	return WeaponAvoidence;
}
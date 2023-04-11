// Fill out your copyright notice in the Description page of Project Settings.

#include "SGTestGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "SquadGameInstance.h"
#include "SquadCameraManager.h"
#include "SquadController.h"
#include "DebugWidget.h"
#include "BattleWidget.h"
#include "BattleTrigger.h"
#include "PlayerSquadCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"



ASGTestGameMode::ASGTestGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("Blueprint'/Game/BLUEPRINT/SquadCameraManager.SquadCameraManager_C'"));
	if (PlayerPawnBPClass.Class)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	else
	{
		DefaultPawnClass = nullptr;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> BattleStartWidget(L"WidgetBlueprint'/Game/UI/BattleWidget.BattleWidget_C'");
	if (BattleStartWidget.Succeeded())
	{
		BattleStartWidgetClass = BattleStartWidget.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> DecisionWidget(L"WidgetBlueprint'/Game/UI/DecisionWidget.DecisionWidget_C'");
	if (DecisionWidget.Succeeded())
	{
		DecisionWidgetClass = DecisionWidget.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> VictoryWidget(L"WidgetBlueprint'/Game/UI/VictoryWidgetBP.VictoryWidgetBP_C'");
	if (VictoryWidget.Succeeded())
	{
		VictoryWidgetClass = VictoryWidget.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> DefeatWidget(L"WidgetBlueprint'/Game/UI/DefeatWidgetBP.DefeatWidgetBP_C'");
	if (DefeatWidget.Succeeded())
	{
		DefeatWidgetClass = DefeatWidget.Class;
	}

}

void ASGTestGameMode::InitGame(const FString& MapName, const FString& option, FString& ErrorMessage)
{
	Super::InitGame(MapName, option, ErrorMessage);

	//Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->InitInstance();
}

void ASGTestGameMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();



	//GetFriendlyChar();
}

void ASGTestGameMode::GetFriendlyChar()
{
	UGameplayStatics::GetAllActorsOfClass(this, APlayerSquadCharacter::StaticClass(), PreFriendlyCharList);
	UGameplayStatics::GetAllActorsOfClass(this, ASquadCameraManager::StaticClass(), tempSCMList);

	SortFrindlyCharList();
	SetTeamPosition();

	SetCharacterState(); //

	Cast<ASquadCameraManager>(tempSCMList[0])->FriendlyCharList.Append(PreFriendlyCharList);
}

void ASGTestGameMode::SetTeamPosition()
{
	int32 chaNum = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->GetCharSlotNum();

	for (int32 i = 0; i < Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->GetCharSlotMaxNum() - chaNum; i++)
	{
		AActor* tempChar = PreFriendlyCharList.Pop();
		tempChar->Destroy();
	}
}

void ASGTestGameMode::SortFrindlyCharList()
{
	for (int32 i = 0; i < PreFriendlyCharList.Num(); i++)
	{
		for (int32 j = 0; j < PreFriendlyCharList.Num(); j++)
		{

			if (Cast<APlayerSquadCharacter>(PreFriendlyCharList[j])->BattleLineNumber == i)
			{
				if (i == j)
				{
					Cast<APlayerSquadCharacter>(PreFriendlyCharList[j])->numbering = i;
				}
				else
				{
					Cast<APlayerSquadCharacter>(PreFriendlyCharList[j])->numbering = i;
					PreFriendlyCharList.Swap(j, i);
				}
			}
		}
	}
}

void ASGTestGameMode::SetCharacterState()
{
	USquadGameInstance* gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

	for (int32 i = 0; i < PreFriendlyCharList.Num(); i++)
	{
		//Cast<APlayerSquadCharacter>(PreFriendlyCharList[i])->Damage = gameIns->CharSlot.CharacterDataArry[i].Damage;
		//Cast<APlayerSquadCharacter>(PreFriendlyCharList[i])->LifePoint = gameIns->CharSlot.CharacterDataArry[i].LifePoint;
		//Cast<APlayerSquadCharacter>(PreFriendlyCharList[i])->ClassNum = gameIns->CharSlot.CharacterDataArry[i].ClassNumber;

		SetCharacterMesh(Cast<APlayerSquadCharacter>(PreFriendlyCharList[i]), Cast<APlayerSquadCharacter>(PreFriendlyCharList[i])->ClassNum);
	}


}

void ASGTestGameMode::SetCharacterMesh(class APlayerSquadCharacter* Character, int32 classNumber)
{

	if (classNumber == 0)
	{
		UE_LOG(LogClass, Log, TEXT(" class number 0 "));
		Character->SetContentMesh(Character->GetMesh(), TEXT("/Game/Military/Mesh/Body/SK_Military_Head1.SK_Military_Head1"));
		Character->SetContentMesh(Character->Cap, TEXT("SkeletalMesh'/Game/Military/Mesh/Head/SK_Military_Helmet2.SK_Military_Helmet2'"));
		Character->SetContentMesh(Character->Cap_equip, TEXT("SkeletalMesh'/Game/Military/Mesh/Head/SK_Military_Helmet2_fastering.SK_Military_Helmet2_fastering'"));
		Character->SetContentMesh(Character->Head_equip1, TEXT("SkeletalMesh'/Game/Military/Mesh/Head/SK_Military_Headset1.SK_Military_Headset1'"));
		Character->SetContentMesh(Character->Head_equip2, TEXT("SkeletalMesh'/Game/Military/Mesh/Head/SK_Military_Goggles1.SK_Military_Goggles1'"));
		Character->SetContentMesh(Character->Shirt, TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Shirt5.SK_Military_Shirt5'"));
		Character->SetContentMesh(Character->Pants, TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Pants4.SK_Military_Pants4'"));
		Character->SetContentMesh(Character->Boots, TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Boots5.SK_Military_Boots5'"));
		Character->SetContentMesh(Character->Hands, TEXT("SkeletalMesh'/Game/Military/Mesh/Body/SK_Military_Hands_Male.SK_Military_Hands_Male'"));
		Character->SetContentMesh(Character->Vest, TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Vest1.SK_Military_Vest1'"));
		Character->SetContentMesh(Character->Vest_outfit, TEXT("SkeletalMesh'/Game/Military/Mesh/Outfit/SK_Military_Outfit9.SK_Military_Outfit9'"));
		Character->SetContentMesh(Character->Backpack, nullptr);
		Character->SetContentMesh(Character->Backpack2, nullptr);
		Character->SetContentMesh(Character->Vest_Collar, nullptr);
		Character->SetContentMesh(Character->Vest_shoulder_R, nullptr);
		Character->SetContentMesh(Character->Vest_shoulder_L, nullptr);
		Character->SetContentMesh(Character->Vest_Bottom, nullptr);
		Character->SetContentMesh(Character->Decals, nullptr);
		Character->SetContentMesh(Character->Radio, TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Radio3.SK_Military_Radio3'"));
		Character->SetContentMesh(Character->Kneepad_R, nullptr);
		Character->SetContentMesh(Character->Kneepad_L, nullptr);
		Character->SetContentMesh(Character->Holster, nullptr);

		//Character->SetWeaponMesh();
		/*
			SkeletalMeshArray[0]->Cap,
			SkeletalMeshArray[0]->Cap_equip,
			SkeletalMeshArray[0]->Head_equip1,
			SkeletalMeshArray[0]->Head_equip2,
			SkeletalMeshArray[0]->Shirt,
			SkeletalMeshArray[0]->Pants,
			SkeletalMeshArray[0]->Boots,
			SkeletalMeshArray[0]->Hands,
			SkeletalMeshArray[0]->Vest,
			SkeletalMeshArray[0]->Vest_outfit,
			SkeletalMeshArray[0]->Backpack,
			SkeletalMeshArray[0]->Backpack2,
			SkeletalMeshArray[0]->Vest_collar,
			SkeletalMeshArray[0]->Vest_shoulder_R,
			SkeletalMeshArray[0]->Vest_shoulder_L,
			SkeletalMeshArray[0]->Vest_bottom,
			SkeletalMeshArray[0]->Decals,
			SkeletalMeshArray[0]->Radio,
			SkeletalMeshArray[0]->Kneepad_R,
			SkeletalMeshArray[0]->Kneepad_L,
			SkeletalMeshArray[0]->Holster);
			*/
	}
	else if (classNumber == 1)
	{

		UE_LOG(LogClass, Log, TEXT(" class number 1 "));
		Character->SetContentMesh(Character->GetMesh(), TEXT("SkeletalMesh'/Game/Military/Mesh/Body/SK_Military_Head2.SK_Military_Head2'"));
		Character->SetContentMesh(Character->Cap, TEXT("SkeletalMesh'/Game/Military/Mesh/Head/SK_Military_Cap7.SK_Military_Cap7'"));
		Character->SetContentMesh(Character->Cap_equip, nullptr);
		Character->SetContentMesh(Character->Head_equip1, nullptr);
		Character->SetContentMesh(Character->Head_equip2, nullptr);
		Character->SetContentMesh(Character->Shirt, TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Shirt5.SK_Military_Shirt5'"));
		Character->SetContentMesh(Character->Pants, TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Pants1.SK_Military_Pants1'"));
		Character->SetContentMesh(Character->Boots, TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Boots5.SK_Military_Boots5'"));
		Character->SetContentMesh(Character->Hands, TEXT("SkeletalMesh'/Game/Military/Mesh/Body/SK_Military_Hands_Male.SK_Military_Hands_Male'"));
		Character->SetContentMesh(Character->Vest, TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Vest1.SK_Military_Vest1'"));
		Character->SetContentMesh(Character->Vest_outfit, TEXT("SkeletalMesh'/Game/Military/Mesh/Outfit/SK_Military_Outfit1.SK_Military_Outfit1'"));
		Character->SetContentMesh(Character->Backpack, TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Backpack3.SK_Military_Backpack3'"));
		Character->SetContentMesh(Character->Backpack2, nullptr);
		Character->SetContentMesh(Character->Vest_Collar, nullptr);
		Character->SetContentMesh(Character->Vest_shoulder_R, nullptr);
		Character->SetContentMesh(Character->Vest_shoulder_L, nullptr);
		Character->SetContentMesh(Character->Vest_Bottom, nullptr);
		Character->SetContentMesh(Character->Decals, nullptr);
		Character->SetContentMesh(Character->Radio, TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Radio7.SK_Military_Radio7'"));
		Character->SetContentMesh(Character->Kneepad_R, TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Kneepad_R.SK_Military_Kneepad_R'"));
		Character->SetContentMesh(Character->Kneepad_L, nullptr);
		Character->SetContentMesh(Character->Holster, TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Vest_Belt.SK_Military_Vest_Belt'"));

		/*
		Character->SetSkeletalMeshComp(SkeletalMeshArray[1]->Head,
			SkeletalMeshArray[1]->Cap,
			SkeletalMeshArray[1]->Cap_equip,
			SkeletalMeshArray[1]->Head_equip1,
			SkeletalMeshArray[1]->Head_equip2,
			SkeletalMeshArray[1]->Shirt,
			SkeletalMeshArray[1]->Pants,
			SkeletalMeshArray[1]->Boots,
			SkeletalMeshArray[1]->Hands,
			SkeletalMeshArray[1]->Vest,
			SkeletalMeshArray[1]->Vest_outfit,
			SkeletalMeshArray[1]->Backpack,
			SkeletalMeshArray[1]->Backpack2,
			SkeletalMeshArray[1]->Vest_collar,
			SkeletalMeshArray[1]->Vest_shoulder_R,
			SkeletalMeshArray[1]->Vest_shoulder_L,
			SkeletalMeshArray[1]->Vest_bottom,
			SkeletalMeshArray[1]->Decals,
			SkeletalMeshArray[1]->Radio,
			SkeletalMeshArray[1]->Kneepad_R,
			SkeletalMeshArray[1]->Kneepad_L,
			SkeletalMeshArray[1]->Holster);
			*/
	}
	else if (classNumber == 2)
	{
		UE_LOG(LogClass, Log, TEXT(" class number 1 "));
		Character->SetContentMesh(Character->GetMesh(), TEXT("SkeletalMesh'/Game/Military/Mesh/Body/SK_Military_Head5.SK_Military_Head5'"));
		Character->SetContentMesh(Character->Cap, TEXT("SkeletalMesh'/Game/Military/Mesh/Head/SK_Military_Cap5.SK_Military_Cap5'"));
		Character->SetContentMesh(Character->Cap_equip, nullptr);
		Character->SetContentMesh(Character->Head_equip1, nullptr);
		Character->SetContentMesh(Character->Head_equip2, nullptr);
		Character->SetContentMesh(Character->Shirt, TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Shirt6.SK_Military_Shirt6'"));
		Character->SetContentMesh(Character->Pants, TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Pants5.SK_Military_Pants5'"));
		Character->SetContentMesh(Character->Boots, TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Boots3.SK_Military_Boots3'"));
		Character->SetContentMesh(Character->Hands, TEXT("SkeletalMesh'/Game/Military/Mesh/Body/SK_Military_Palm_Hands_Male.SK_Military_Palm_Hands_Male'"));
		Character->SetContentMesh(Character->Vest, TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Heavy_Vest.SK_Military_Heavy_Vest'"));
		Character->SetContentMesh(Character->Vest_outfit, TEXT("SkeletalMesh'/Game/Military/Mesh/Outfit/SK_Military_Outfit6.SK_Military_Outfit6'"));
		Character->SetContentMesh(Character->Backpack, nullptr);
		Character->SetContentMesh(Character->Backpack2, nullptr);
		Character->SetContentMesh(Character->Vest_Collar, TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Heavy_Vest_collar.SK_Military_Heavy_Vest_collar'"));
		Character->SetContentMesh(Character->Vest_shoulder_R, nullptr);
		Character->SetContentMesh(Character->Vest_shoulder_L, nullptr);
		Character->SetContentMesh(Character->Vest_Bottom, nullptr);
		Character->SetContentMesh(Character->Decals, nullptr);
		Character->SetContentMesh(Character->Radio, TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Radio4.SK_Military_Radio4'"));
		Character->SetContentMesh(Character->Kneepad_R, TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Kneepad_L.SK_Military_Kneepad_L'"));
		Character->SetContentMesh(Character->Kneepad_L, TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Vest_Belt.SK_Military_Vest_Belt'"));
		Character->SetContentMesh(Character->Holster, nullptr);
		/*
		Character->SetSkeletalMeshComp(SkeletalMeshArray[2]->Head,
			SkeletalMeshArray[2]->Cap,
			SkeletalMeshArray[2]->Cap_equip,
			SkeletalMeshArray[2]->Head_equip1,
			SkeletalMeshArray[2]->Head_equip2,
			SkeletalMeshArray[2]->Shirt,
			SkeletalMeshArray[2]->Pants,
			SkeletalMeshArray[2]->Boots,
			SkeletalMeshArray[2]->Hands,
			SkeletalMeshArray[2]->Vest,
			SkeletalMeshArray[2]->Vest_outfit,
			SkeletalMeshArray[2]->Backpack,
			SkeletalMeshArray[2]->Backpack2,
			SkeletalMeshArray[2]->Vest_collar,
			SkeletalMeshArray[2]->Vest_shoulder_R,
			SkeletalMeshArray[2]->Vest_shoulder_L,
			SkeletalMeshArray[2]->Vest_bottom,
			SkeletalMeshArray[2]->Decals,
			SkeletalMeshArray[2]->Radio,
			SkeletalMeshArray[2]->Kneepad_R,
			SkeletalMeshArray[2]->Kneepad_L,
			SkeletalMeshArray[2]->Holster);
			*/
	}
	else
	{

	}

}

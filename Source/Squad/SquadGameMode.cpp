// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
#define DEBUG 0

#include "SquadGameMode.h"
#include "SquadCameraManager.h"
#include "SquadController.h"
#include "DebugWidget.h"
#include "BattleWidget.h"
//#include "DecisionWidget.h"
#include "DecisionResultWidget.h"
#include "BattleTrigger.h"
#include "PlayerSquadCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "SquadCharacterStatComponent.h"
#include "SquadGameInstance.h"
#include "Engine/Engine.h"

ASquadGameMode::ASquadGameMode()
{
	// set default pawn class to our Blueprinted character

	FString path = L"";
	path = L"Blueprint'/Game/BLUEPRINT/SquadCameraManager.SquadCameraManager_C'";

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(*path);
	if (PlayerPawnBPClass.Succeeded())
	{
		//DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	else
	{
		DefaultPawnClass = NULL;
	}

	static ConstructorHelpers::FClassFinder<ASquadController> SquadController(TEXT("Blueprint'/Game/BLUEPRINT/SquadControllerBP.SquadControllerBP_C'"));
	if (SquadController.Succeeded())
	{
		PlayerControllerClass = SquadController.Class;
	}
	//PlayerControllerClass

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

	static ConstructorHelpers::FClassFinder<UUserWidget> DecisionBossWidget(L"WidgetBlueprint'/Game/UI/DecisionBossWidget.DecisionBossWidget_C'");
	if (DecisionBossWidget.Succeeded())
	{
		DecisionBossWidgetClass = DecisionBossWidget.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> DecisionBossResultWidget(L"WidgetBlueprint'/Game/UI/DecisionBossResultWidget.DecisionBossResultWidget_C'");
	if (DecisionBossResultWidget.Succeeded())
	{
		DecisionBossResultWidgetClass = DecisionBossResultWidget.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> DecisionResultWidget(L"WidgetBlueprint'/Game/UI/DecisionResultWidget.DecisionResultWidget_C'");
	if (DecisionResultWidget.Succeeded())
	{
		DecisionResultWidgetClass = DecisionResultWidget.Class;
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

	static ConstructorHelpers::FClassFinder<UUserWidget> ExploreWidget(L"WidgetBlueprint'/Game/Map/M_PlayLevel_Widget/UW_PlayerLevel_exploreLevel.UW_PlayerLevel_exploreLevel_C'");
	if (ExploreWidget.Succeeded())
	{
		ExploreWidgetClass = ExploreWidget.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> GameVictoryWidget(L"WidgetBlueprint'/Game/UI/GameVictoryWidgetBP.GameVictoryWidgetBP_C'");
	if (GameVictoryWidget.Succeeded())
	{
		GameVictoryWidgetClass = GameVictoryWidget.Class;
	}

	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> ClassNumber_0_Mesh(L"SkeletalMesh'/Game/Characters/Modular_soldier_01/Meshes/SM_Modular_soldier_14.SM_Modular_soldier_14'");
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> ClassNumber_1_Mesh(L"SkeletalMesh'/Game/Characters/Modular_soldier_01/Meshes/SM_Modular_soldier_14.SM_Modular_soldier_14'");
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> ClassNumber_2_Mesh(L"SkeletalMesh'/Game/Characters/Modular_soldier_01/Meshes/SM_Modular_soldier_14.SM_Modular_soldier_14'");

	//	static ConstructorHelpers::FObjectFinder<

	//Class_0_Mesh_Load();
	//Class_1_Mesh_Load();
	//Class_2_Mesh_Load();
}


// 
void ASquadGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);


	//Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->InitInstance(); // 인스턴스 변수 초기화
}

void ASquadGameMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	


	//GetFriendlyChar();


}

void ASquadGameMode::GetFriendlyChar()
{
	if (PreFriendlyCharList.Num() > 0)
	{
		PreFriendlyCharList.Empty();
		tempSCMList.Empty();
		PlayerCharList.Empty();
	}

	UGameplayStatics::GetAllActorsOfClass(this, APlayerSquadCharacter::StaticClass(), PreFriendlyCharList); // 배치한 캐릭터
	UGameplayStatics::GetAllActorsOfClass(this, ASquadCameraManager::StaticClass(), tempSCMList); // 카메라 액터

	UE_LOG(LogClass, Log, TEXT(" %d "), PreFriendlyCharList.Num()); 
	UE_LOG(LogClass, Log, TEXT(" %d "), tempSCMList.Num()); 
	
	

	SortFrindlyCharList(); // 캐릭터 정해진 순번대로 정렬
	preSetCharacterState();
	//SetTeamPosition();
	
	SetCharacterState();  // 문제

	
	PlayerCharList = PreFriendlyCharList;
	


	Cast<ASquadCameraManager>(tempSCMList[0])->GetFriendlyChar();

	SetStartGamelevel();
	
	
}

void ASquadGameMode::preSetCharacterState()
{
	auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	TArray<FCharacterDataStruct> tempArray;
	tempArray.Append(gameIns->CharSlot.CharacterDataArry);
	
	int32 chaNum = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->GetCharacterDataNum();


	for (int32 i = 0; i < Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->CharSlot.MaxSlotNum - chaNum; i++)
	{
		AActor* tempChar = PreFriendlyCharList.Pop();
		tempChar->Destroy();
	}	

	int32 numberofChar = PreFriendlyCharList.Num();
	gameIns->SCMIns->SetUnitPos_Last_Location(chaNum);
}

void ASquadGameMode::SaveCharacterData()
{
	auto SC = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SCMIns;
	auto FrindlyCharacterList = SC->FriendlyCharList;


	auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	gameIns->CharSlot.EmptyCharacterDataArry();

	for(int i = 0 ; i < FrindlyCharacterList.Num() ; i++)
	{
		FCharacterDataStruct tempDataStruct;
		tempDataStruct.LifePoint = Cast<APlayerSquadCharacter>(FrindlyCharacterList[i])->LifePoint;
		tempDataStruct.ClassNumber = Cast<APlayerSquadCharacter>(FrindlyCharacterList[i])->ClassNum;
		tempDataStruct.IsExist = true;
		gameIns->IncludeCharacterData(tempDataStruct);
	}



}

void ASquadGameMode::CheckArrayNum()
{
	auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	   	  	
	// 게임 인스 데이터
	float A = gameIns->GetCharacterDataNum();
	// 모드 데이터
	float B = PreFriendlyCharList.Num();
	float C = PlayerCharList.Num();
	// 카메라 데이터
	float D = Cast<ASquadCameraManager>(tempSCMList[0])->FriendlyCharList.Num();	
}

void ASquadGameMode::SetStartGamelevel()
{
	ViewBattleWidget();
	Cast<UBattleWidget>(CurrentWidget)->Set_BattleWidgetOpacity(1.f);



	DisplayExplerWidget = CreateWidget(GetWorld(), ExploreWidgetClass);
	if (DisplayExplerWidget != nullptr)
	{
		DisplayExplerWidget->AddToViewport();
	}
}

UUserWidget* ASquadGameMode::GetExploreWidget()
{
	return DisplayExplerWidget;
}

void ASquadGameMode::SetTeamPosition()
{
	
	int32 chaNum = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->GetCharacterDataNum();
		
	
	for(int32 i = 0 ; i < Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->CharSlot.MaxSlotNum - chaNum; i++)
	{
		AActor* tempChar = PreFriendlyCharList.Pop();
		tempChar->Destroy();
	}
	
	
}

void ASquadGameMode::SortFrindlyCharList()
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

void ASquadGameMode::SetCharacterState() // 캐릭터 능력치 적용
{
	USquadGameInstance* gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

	for (int32 i = 0; i < PreFriendlyCharList.Num(); i++)
	{
						
		// 2번째 실행때 gameIns->CharSlot.CharacterDataArry의 num이 0이 되어버려서 크래쉬가 발생
		// 문제는 인스턴스에 있는 캐릭터데이터배열의 데이터가 추가가 안되는것이 문제

		Cast<APlayerSquadCharacter>(PreFriendlyCharList[i])->ClassNum = gameIns->CharSlot.CharacterDataArry[i].ClassNumber;
		SetCharacterMesh(Cast<APlayerSquadCharacter>(PreFriendlyCharList[i]), Cast<APlayerSquadCharacter>(PreFriendlyCharList[i])->ClassNum);
		if (gameIns->CharSlot.CharacterDataArry[i].LifePoint != NULL)
			Cast<APlayerSquadCharacter>(PreFriendlyCharList[i])->LifePoint = gameIns->CharSlot.CharacterDataArry[i].LifePoint;
#if DEBUG == true
			
		//Cast<APlayerSquadCharacter>(PreFriendlyCharList[i])->

#endif
	
	}


}

void ASquadGameMode::ClearInstanceCharacterStat()
{
	USquadGameInstance* gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

	gameIns->CharSlot.CharacterDataArry.Empty();
}

void ASquadGameMode::SetCharacterMesh(class APlayerSquadCharacter* Character, int32 classNumber)
{
	
	if (classNumber == 0) // Rifleman
	{
		Character->SetContentMesh(Character->GetMesh(), TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Body/SK_Military_Head1.SK_Military_Head1'"));
		Character->SetContentMesh(Character->Cap, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Head/SK_Military_Helmet1.SK_Military_Helmet1'")); 
		Character->SetContentMesh(Character->Cap_equip, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Head/SK_Military_Helmet2_fastering.SK_Military_Helmet2_fastering'")); 
		Character->SetContentMesh(Character->Head_equip1, nullptr);
		Character->SetContentMesh(Character->Head_equip2, nullptr);
		Character->SetContentMesh(Character->Shirt, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Shirt6.SK_Military_Shirt6'")); 
		Character->SetContentMesh(Character->Pants, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Pants4.SK_Military_Pants4'")); 
		Character->SetContentMesh(Character->Boots, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Boots4.SK_Military_Boots4'"));
		Character->SetContentMesh(Character->Hands, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Body/SK_Military_Gloves_Male.SK_Military_Gloves_Male'"));
		Character->SetContentMesh(Character->Vest, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Vest1.SK_Military_Vest1'")); 
		Character->SetContentMesh(Character->Vest_outfit, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Outfit/SK_Military_Outfit1.SK_Military_Outfit1'")); 
		Character->SetContentMesh(Character->Backpack, nullptr);
		Character->SetContentMesh(Character->Backpack2, nullptr);
		Character->SetContentMesh(Character->Vest_Collar, nullptr);
		Character->SetContentMesh(Character->Vest_shoulder_R, nullptr);
		Character->SetContentMesh(Character->Vest_shoulder_L, nullptr);
		Character->SetContentMesh(Character->Vest_Bottom, nullptr);
		Character->SetContentMesh(Character->Decals, nullptr);
		Character->SetContentMesh(Character->Radio, nullptr);
		Character->SetContentMesh(Character->Kneepad_R, nullptr);
		Character->SetContentMesh(Character->Kneepad_L, nullptr);
		Character->SetContentMesh(Character->Holster, nullptr);
		
		Character->SetContentMeshMat(Character->GetMesh(), TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Head1.Mat_Military_Head1'"), 0);  
		Character->SetContentMeshMat(Character->GetMesh(), TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Eyes.Mat_Military_Eyes'"), 1);
		Character->SetContentMeshMat(Character->Cap, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Cap1_skin3.Mat_Military_Cap1_skin3'") , 0); 
		Character->SetContentMeshMat(Character->Cap_equip, TEXT(" Material'/Game/Characters/Military/Materials/Mat_Military_Helmet2_skin1.Mat_Military_Helmet2_skin1'"), 0);
		Character->SetContentMeshMat(Character->Head_equip1, nullptr,0);
		Character->SetContentMeshMat(Character->Head_equip2, nullptr,0);
		Character->SetContentMeshMat(Character->Shirt, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Shirt6_skin1.Mat_Military_Shirt6_skin1'"),0);  
		Character->SetContentMeshMat(Character->Pants, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Pants4_skin2.Mat_Military_Pants4_skin2'"),0); 
		Character->SetContentMeshMat(Character->Boots, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Boots4-5_skin1.Mat_Military_Boots4-5_skin1'"),0); 
		Character->SetContentMeshMat(Character->Hands, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Hands1_skin1.Mat_Military_Hands1_skin1'"),0); 
		Character->SetContentMeshMat(Character->Hands, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Equip2_skin1.Mat_Military_Equip2_skin1'"),1);
		Character->SetContentMeshMat(Character->Vest, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Vest1_skin2.Mat_Military_Vest1_skin2'")); 
		Character->SetContentMeshMat(Character->Vest_outfit, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Vest1_skin1.Mat_Military_Vest1_skin1'")); 
		Character->SetContentMeshMat(Character->Vest_outfit, TEXT(" Material'/Game/Characters/Military/Materials/Mat_Military_Equip2_skin1.Mat_Military_Equip2_skin1'") ,1);
		Character->SetContentMeshMat(Character->Vest_outfit, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Pouches1_skin2.Mat_Military_Pouches1_skin2'"), 2);
		Character->SetContentMeshMat(Character->Backpack, nullptr,0);
		Character->SetContentMeshMat(Character->Backpack2, nullptr,0);
		Character->SetContentMeshMat(Character->Vest_Collar, nullptr,0);
		Character->SetContentMeshMat(Character->Vest_shoulder_R, nullptr,0);
		Character->SetContentMeshMat(Character->Vest_shoulder_L, nullptr,0);
		Character->SetContentMeshMat(Character->Vest_Bottom, nullptr,0);
		Character->SetContentMeshMat(Character->Decals, nullptr,0);
		Character->SetContentMeshMat(Character->Radio, nullptr,0);
		Character->SetContentMeshMat(Character->Kneepad_R, nullptr,0);
		Character->SetContentMeshMat(Character->Kneepad_L, nullptr,0);
		Character->SetContentMeshMat(Character->Holster, nullptr,0);

		Character->CharacterStat->SetClassData(classNumber);
		
		Character->SetFXSound(TEXT("SoundWave'/Game/AUDIO/Sound/Voice/HumanMaleB/voice_male_b_hurt_pain_set_5_14.voice_male_b_hurt_pain_set_5_14'"),
			TEXT("SoundWave'/Game/AUDIO/Sound/Voice/HumanMaleB/voice_male_b_death_torture_13.voice_male_b_death_torture_13'"));
		//Character->CharacterStat->SetWeaponData(0);
		//Character->SetContentMesh(Character->WeaponSlot, TEXT("SkeletalMesh'/Game/MilitaryWeapDark/Weapons/Assault_Rifle_B.Assault_Rifle_B'"));
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
	else if (classNumber == 1)  // Deminer
	{
		Character->SetContentMesh(Character->GetMesh(), TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Body/SK_Military_Head6.SK_Military_Head6'"));
		Character->SetContentMesh(Character->Cap, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Head/SK_Military_Helmet3.SK_Military_Helmet3'"));
		Character->SetContentMesh(Character->Cap_equip, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Head/SK_Military_Helmet3_fastering.SK_Military_Helmet3_fastering'"));
		Character->SetContentMesh(Character->Head_equip1, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Head/SK_Military_Helmet3_visor-close.SK_Military_Helmet3_visor-close'"));
		Character->SetContentMesh(Character->Head_equip2, nullptr);
		Character->SetContentMesh(Character->Shirt, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Shirt6.SK_Military_Shirt6'"));
		Character->SetContentMesh(Character->Pants, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Pants1.SK_Military_Pants1'"));
		Character->SetContentMesh(Character->Boots, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Boots4.SK_Military_Boots4'"));
		Character->SetContentMesh(Character->Hands, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Body/SK_Military_Gloves_Male.SK_Military_Gloves_Male'"));
		Character->SetContentMesh(Character->Vest, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Heavy_Vest.SK_Military_Heavy_Vest'"));
		Character->SetContentMesh(Character->Vest_outfit, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Outfit/SK_Military_Outfit5.SK_Military_Outfit5'"));
		Character->SetContentMesh(Character->Backpack, nullptr);
		Character->SetContentMesh(Character->Backpack2, nullptr);
		Character->SetContentMesh(Character->Vest_Collar, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Heavy_Vest_collar.SK_Military_Heavy_Vest_collar'"));
		Character->SetContentMesh(Character->Vest_shoulder_R, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Heavy_Vest_Shoulder_R.SK_Military_Heavy_Vest_Shoulder_R'"));
		Character->SetContentMesh(Character->Vest_shoulder_L, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Heavy_Vest_Shoulder_L.SK_Military_Heavy_Vest_Shoulder_L'"));
		Character->SetContentMesh(Character->Vest_Bottom, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Heavy_Vest_Bottom.SK_Military_Heavy_Vest_Bottom'"));
		Character->SetContentMesh(Character->Decals, nullptr);
		Character->SetContentMesh(Character->Radio, nullptr);
		Character->SetContentMesh(Character->Kneepad_R, nullptr);
		Character->SetContentMesh(Character->Kneepad_L, nullptr);
		Character->SetContentMesh(Character->Holster, nullptr);

		Character->SetContentMeshMat(Character->GetMesh(), TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Head6.Mat_Military_Head6'"), 0);
		Character->SetContentMeshMat(Character->Cap, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Helmet3_skin1.Mat_Military_Helmet3_skin1'"), 0);
		Character->SetContentMeshMat(Character->Cap_equip, TEXT(" Material'/Game/Characters/Military/Materials/Mat_Military_Helmet3_skin1.Mat_Military_Helmet3_skin1'"), 0);
		Character->SetContentMeshMat(Character->Head_equip1, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Helmet3_skin1.Mat_Military_Helmet3_skin1'"), 0);
		Character->SetContentMeshMat(Character->Head_equip1, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Glass.Mat_Military_Glass'"), 1);
		Character->SetContentMeshMat(Character->Head_equip2, nullptr, 0);
		Character->SetContentMeshMat(Character->Shirt, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Shirt6_skin1.Mat_Military_Shirt6_skin1'"), 0);
		Character->SetContentMeshMat(Character->Pants, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Pants1_skin1.Mat_Military_Pants1_skin1'"), 0);
		Character->SetContentMeshMat(Character->Boots, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Boots4-5_skin1.Mat_Military_Boots4-5_skin1'"), 0);
		Character->SetContentMeshMat(Character->Hands, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Hands2_skin2.Mat_Military_Hands2_skin2'"), 0);
		Character->SetContentMeshMat(Character->Hands, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Equip2_skin1.Mat_Military_Equip2_skin1'"), 1);
		Character->SetContentMeshMat(Character->Vest, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Vest2_skin1.Mat_Military_Vest2_skin1'"));
		Character->SetContentMeshMat(Character->Vest_outfit, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Pouches1_skin2.Mat_Military_Pouches1_skin2'"));
		Character->SetContentMeshMat(Character->Backpack, nullptr, 0);
		Character->SetContentMeshMat(Character->Backpack2, nullptr, 0);
		Character->SetContentMeshMat(Character->Vest_Collar, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Vest2_skin1.Mat_Military_Vest2_skin1'"));
		Character->SetContentMeshMat(Character->Vest_shoulder_R, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Vest2_skin1.Mat_Military_Vest2_skin1'"), 0);
		Character->SetContentMeshMat(Character->Vest_shoulder_L, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Vest2_skin1.Mat_Military_Vest2_skin1'"), 0);
		Character->SetContentMeshMat(Character->Vest_Bottom, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Vest2_skin1.Mat_Military_Vest2_skin1'"), 0);
		Character->SetContentMeshMat(Character->Decals, nullptr, 0);
		Character->SetContentMeshMat(Character->Radio, nullptr, 0);
		Character->SetContentMeshMat(Character->Kneepad_R, nullptr, 0);
		Character->SetContentMeshMat(Character->Kneepad_L, nullptr, 0);
		Character->SetContentMeshMat(Character->Holster, nullptr, 0);

		Character->CharacterStat->SetClassData(classNumber);
//		Character->CharacterStat->SetWeaponData(1);
		//Character->SetContentMesh(Character->WeaponSlot, TEXT("SkeletalMesh'/Game/MilitaryWeapDark/Weapons/Pistols_B.Pistols_B'"));
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
		UE_LOG(LogClass, Log, TEXT(" class number 2 "));
		Character->SetContentMesh(Character->GetMesh(), TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Body/SK_Military_Head5.SK_Military_Head5'"));
		Character->SetContentMesh(Character->Cap, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Head/SK_Military_Cap5.SK_Military_Cap5'"));
		Character->SetContentMesh(Character->Cap_equip, nullptr);
		Character->SetContentMesh(Character->Head_equip1, nullptr);
		Character->SetContentMesh(Character->Head_equip2, nullptr);
		Character->SetContentMesh(Character->Shirt, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Shirt6.SK_Military_Shirt6'"));
		Character->SetContentMesh(Character->Pants, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Pants5.SK_Military_Pants5'"));
		Character->SetContentMesh(Character->Boots, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Boots3.SK_Military_Boots3'"));
		Character->SetContentMesh(Character->Hands, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Body/SK_Military_Palm_Hands_Male.SK_Military_Palm_Hands_Male'"));
		Character->SetContentMesh(Character->Vest, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Heavy_Vest.SK_Military_Heavy_Vest'"));
		Character->SetContentMesh(Character->Vest_outfit, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Outfit/SK_Military_Outfit6.SK_Military_Outfit6'"));
		Character->SetContentMesh(Character->Backpack, nullptr);
		Character->SetContentMesh(Character->Backpack2, nullptr);
		Character->SetContentMesh(Character->Vest_Collar, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Heavy_Vest_collar.SK_Military_Heavy_Vest_collar'"));
		Character->SetContentMesh(Character->Vest_shoulder_R, nullptr);
		Character->SetContentMesh(Character->Vest_shoulder_L, nullptr);
		Character->SetContentMesh(Character->Vest_Bottom, nullptr);
		Character->SetContentMesh(Character->Decals, nullptr);
		Character->SetContentMesh(Character->Radio, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Radio4.SK_Military_Radio4'"));
		Character->SetContentMesh(Character->Kneepad_R, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Kneepad_L.SK_Military_Kneepad_L'"));
		Character->SetContentMesh(Character->Kneepad_L,TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Vest_Belt.SK_Military_Vest_Belt'"));
		Character->SetContentMesh(Character->Holster, nullptr);



		Character->CharacterStat->SetClassData(classNumber);
//		Character->CharacterStat->SetWeaponData(2);
		//	Character->SetContentMesh(Character->WeaponSlot, TEXT("SkeletalMesh'/Game/MilitaryWeapDark/Weapons/Shotgun_B.Shotgun_B'"));
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
	else if (classNumber == 3) // police
	{
		Character->SetContentMesh(Character->GetMesh(), TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Body/SK_Military_Head7.SK_Military_Head7'"));
		Character->SetContentMesh(Character->Cap, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Head/SK_Military_Helmet2_for_goggles.SK_Military_Helmet2_for_goggles'"));
		Character->SetContentMesh(Character->Cap_equip, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Head/SK_Military_Helmet2_fastering.SK_Military_Helmet2_fastering'"));
		Character->SetContentMesh(Character->Head_equip1, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Head/SK_Military_Goggles1.SK_Military_Goggles1'"));
		Character->SetContentMesh(Character->Head_equip2, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Head/SK_Military_Headset1.SK_Military_Headset1'"));
		Character->SetContentMesh(Character->Shirt, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Shirt6.SK_Military_Shirt6'"));
		Character->SetContentMesh(Character->Pants, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Pants5.SK_Military_Pants5'"));
		Character->SetContentMesh(Character->Boots, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Boots5.SK_Military_Boots5'"));
		Character->SetContentMesh(Character->Hands, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Body/SK_Military_Gloves_Female.SK_Military_Gloves_Female'"));
		Character->SetContentMesh(Character->Vest, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Heavy_Vest.SK_Military_Heavy_Vest'"));
		Character->SetContentMesh(Character->Vest_outfit, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Outfit/SK_Military_Outfit7.SK_Military_Outfit7'"));
		Character->SetContentMesh(Character->Backpack, nullptr);
		Character->SetContentMesh(Character->Backpack2, nullptr);
		Character->SetContentMesh(Character->Vest_Collar, nullptr);
		Character->SetContentMesh(Character->Vest_shoulder_R, nullptr);
		Character->SetContentMesh(Character->Vest_shoulder_L, nullptr);
		Character->SetContentMesh(Character->Vest_Bottom, nullptr);
		Character->SetContentMesh(Character->Decals, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Decals/SK_Military_Decal_Police.SK_Military_Decal_Police'"));
		Character->SetContentMesh(Character->Radio, nullptr);
		Character->SetContentMesh(Character->Kneepad_R, nullptr);
		Character->SetContentMesh(Character->Kneepad_L, nullptr);
		Character->SetContentMesh(Character->Holster, nullptr);

		Character->SetContentMeshMat(Character->GetMesh(), TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Head7_skin1.Mat_Military_Head7_skin1'"), 0);
		Character->SetContentMeshMat(Character->Cap, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Helmet2_skin3.Mat_Military_Helmet2_skin3'"), 0);
		Character->SetContentMeshMat(Character->Cap_equip, TEXT(" Material'/Game/Characters/Military/Materials/Mat_Military_Helmet2_skin1.Mat_Military_Helmet2_skin1'"), 0);
		Character->SetContentMeshMat(Character->Head_equip1, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Glass.Mat_Military_Glass'"), 0);
		Character->SetContentMeshMat(Character->Head_equip1, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Cap4_skin1.Mat_Military_Cap4_skin1'"), 1);
		Character->SetContentMeshMat(Character->Head_equip2, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Helmet2_skin1.Mat_Military_Helmet2_skin1'"), 0);
		Character->SetContentMeshMat(Character->Shirt, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Shirt6_skin2.Mat_Military_Shirt6_skin2'"), 0);
		Character->SetContentMeshMat(Character->Pants, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Pants5_skin4.Mat_Military_Pants5_skin4'"), 0);
		Character->SetContentMeshMat(Character->Boots, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Boots4-5_skin1.Mat_Military_Boots4-5_skin1'"), 0);
		Character->SetContentMeshMat(Character->Hands, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Hands2_skin1.Mat_Military_Hands2_skin1'"), 0);
		Character->SetContentMeshMat(Character->Hands, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Equip2_skin1.Mat_Military_Equip2_skin1'"), 1);
		Character->SetContentMeshMat(Character->Vest, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Vest2_skin3.Mat_Military_Vest2_skin3'"));
		Character->SetContentMeshMat(Character->Vest_outfit, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Pouches1_skin1.Mat_Military_Pouches1_skin1'"));
		Character->SetContentMeshMat(Character->Vest_outfit, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Equip2_skin1.Mat_Military_Equip2_skin1'"),1);
		Character->SetContentMeshMat(Character->Vest_outfit, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Pistol1_skin1.Mat_Military_Pistol1_skin1'"),2);
		Character->SetContentMeshMat(Character->Backpack, nullptr, 0);
		Character->SetContentMeshMat(Character->Backpack2, nullptr, 0);
		Character->SetContentMeshMat(Character->Vest_Collar, nullptr);
		Character->SetContentMeshMat(Character->Vest_shoulder_R, nullptr,  0);
		Character->SetContentMeshMat(Character->Vest_shoulder_L, nullptr , 0);
		Character->SetContentMeshMat(Character->Vest_Bottom, nullptr , 0);
		Character->SetContentMeshMat(Character->Decals, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Decals.Mat_Military_Decals'"), 0);
		Character->SetContentMeshMat(Character->Radio, nullptr, 0);
		Character->SetContentMeshMat(Character->Kneepad_R, nullptr, 0);
		Character->SetContentMeshMat(Character->Kneepad_L, nullptr, 0);
		Character->SetContentMeshMat(Character->Holster, nullptr, 0);

		Character->CharacterStat->SetClassData(classNumber);
	}
	else if (classNumber == 4) // medic
	{
		Character->SetContentMesh(Character->GetMesh(), TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Body/SK_Military_Head8.SK_Military_Head8'"));
		Character->SetContentMesh(Character->Cap, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Head/SK_Military_Helmet2.SK_Military_Helmet2'"));
		Character->SetContentMesh(Character->Cap_equip, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Head/SK_Military_Helmet2_fastering.SK_Military_Helmet2_fastering'"));
		Character->SetContentMesh(Character->Head_equip1, nullptr);
		Character->SetContentMesh(Character->Head_equip2, nullptr);
		Character->SetContentMesh(Character->Shirt, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Shirt5.SK_Military_Shirt5'"));
		Character->SetContentMesh(Character->Pants, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Pants5.SK_Military_Pants5'"));
		Character->SetContentMesh(Character->Boots, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Boots4.SK_Military_Boots4'"));
		Character->SetContentMesh(Character->Hands, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Body/SK_Military_Hands_Female.SK_Military_Hands_Female'"));
		Character->SetContentMesh(Character->Vest, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Vest1.SK_Military_Vest1'"));
		Character->SetContentMesh(Character->Vest_outfit, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Outfit/SK_Military_Outfit2.SK_Military_Outfit2'"));
		Character->SetContentMesh(Character->Backpack, nullptr);
		Character->SetContentMesh(Character->Backpack2, nullptr);
		Character->SetContentMesh(Character->Vest_Collar, nullptr);
		Character->SetContentMesh(Character->Vest_shoulder_R, nullptr);
		Character->SetContentMesh(Character->Vest_shoulder_L, nullptr);
		Character->SetContentMesh(Character->Vest_Bottom, nullptr);
		Character->SetContentMesh(Character->Decals, nullptr);
		Character->SetContentMesh(Character->Radio, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Decals/SK_Military_Decal_Medic.SK_Military_Decal_Medic'"));
		Character->SetContentMesh(Character->Kneepad_R, nullptr);
		Character->SetContentMesh(Character->Kneepad_L, nullptr);
		Character->SetContentMesh(Character->Holster, nullptr);

		Character->SetContentMeshMat(Character->GetMesh(), TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Head8_skin1.Mat_Military_Head8_skin1'"), 0);
		Character->SetContentMeshMat(Character->Cap, TEXT(" Material'/Game/Characters/Military/Materials/Mat_Military_Helmet2_skin1.Mat_Military_Helmet2_skin1'"), 0);
		Character->SetContentMeshMat(Character->Cap_equip, TEXT(" Material'/Game/Characters/Military/Materials/Mat_Military_Helmet2_skin1.Mat_Military_Helmet2_skin1'"), 0);
		Character->SetContentMeshMat(Character->Head_equip1, nullptr, 0);
		Character->SetContentMeshMat(Character->Head_equip2, nullptr, 0);
		Character->SetContentMeshMat(Character->Shirt, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Shirt5_skin1.Mat_Military_Shirt5_skin1'"), 0);
		Character->SetContentMeshMat(Character->Pants, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Pants5_skin5.Mat_Military_Pants5_skin5'"), 0);
		Character->SetContentMeshMat(Character->Boots, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Boots4-5_skin1.Mat_Military_Boots4-5_skin1'"), 0);
		Character->SetContentMeshMat(Character->Hands, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Hands2_skin1.Mat_Military_Hands2_skin1'"), 0);
		Character->SetContentMeshMat(Character->Vest, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Vest1_skin3.Mat_Military_Vest1_skin3'"));
		Character->SetContentMeshMat(Character->Vest_outfit, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Pouches1_skin3.Mat_Military_Pouches1_skin3'"));
		Character->SetContentMeshMat(Character->Vest_outfit, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Pistol1_skin2.Mat_Military_Pistol1_skin2'"), 1);
		Character->SetContentMeshMat(Character->Vest_outfit, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Equip2_skin3.Mat_Military_Equip2_skin3'"), 2);
		Character->SetContentMeshMat(Character->Backpack, nullptr, 0);
		Character->SetContentMeshMat(Character->Backpack2, nullptr, 0);
		Character->SetContentMeshMat(Character->Vest_Collar, nullptr);
		Character->SetContentMeshMat(Character->Vest_shoulder_R, nullptr, 0);
		Character->SetContentMeshMat(Character->Vest_shoulder_L, nullptr, 0);
		Character->SetContentMeshMat(Character->Vest_Bottom, nullptr, 0);
		Character->SetContentMeshMat(Character->Decals, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Decals.Mat_Military_Decals'"), 0);
		Character->SetContentMeshMat(Character->Radio, nullptr, 0);
		Character->SetContentMeshMat(Character->Kneepad_R, nullptr, 0);
		Character->SetContentMeshMat(Character->Kneepad_L, nullptr, 0);
		Character->SetContentMeshMat(Character->Holster, nullptr, 0);

		Character->CharacterStat->SetClassData(classNumber);
		Character->SetFXSound(nullptr,
			TEXT("SoundWave'/Game/AUDIO/Sound/Voice/HumanMaleA/voice_female_b_death_torture_02.voice_female_b_death_torture_02'"));
	}
	else if (classNumber == 5) // assault
	{
		Character->SetContentMesh(Character->GetMesh(), TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Body/SK_Military_Head2.SK_Military_Head2'"));
		Character->SetContentMesh(Character->Cap, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Head/SK_Military_Helmet1.SK_Military_Helmet1'"));
		Character->SetContentMesh(Character->Cap_equip, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Head/SK_Military_Helmet1_fastering.SK_Military_Helmet1_fastering'"));
		Character->SetContentMesh(Character->Head_equip1, nullptr);
		Character->SetContentMesh(Character->Head_equip2, nullptr);
		Character->SetContentMesh(Character->Shirt, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Shirt5.SK_Military_Shirt5'"));
		Character->SetContentMesh(Character->Pants, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Pants1.SK_Military_Pants1'"));
		Character->SetContentMesh(Character->Boots, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Boots3.SK_Military_Boots3'"));
		Character->SetContentMesh(Character->Hands, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Body/SK_Military_Hands_Male.SK_Military_Hands_Male'"));
		Character->SetContentMesh(Character->Vest, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Heavy_Vest.SK_Military_Heavy_Vest'"));
		Character->SetContentMesh(Character->Vest_outfit, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Outfit/SK_Military_Outfit6.SK_Military_Outfit6'"));
		Character->SetContentMesh(Character->Backpack, nullptr);
		Character->SetContentMesh(Character->Backpack2, nullptr);
		Character->SetContentMesh(Character->Vest_Collar, nullptr);
		Character->SetContentMesh(Character->Vest_shoulder_R, nullptr);
		Character->SetContentMesh(Character->Vest_shoulder_L, nullptr);
		Character->SetContentMesh(Character->Vest_Bottom, nullptr);
		Character->SetContentMesh(Character->Decals, nullptr);
		Character->SetContentMesh(Character->Radio, nullptr);
		Character->SetContentMesh(Character->Kneepad_R, nullptr);
		Character->SetContentMesh(Character->Kneepad_L, nullptr);
		Character->SetContentMesh(Character->Holster, nullptr);

		Character->SetContentMeshMat(Character->GetMesh(), TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Eyes.Mat_Military_Eyes'"), 0);
		Character->SetContentMeshMat(Character->GetMesh(), TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Head2.Mat_Military_Head2'"), 1);
		Character->SetContentMeshMat(Character->Cap, TEXT(" Material'/Game/Characters/Military/Materials/Mat_Military_Cap1_skin2.Mat_Military_Cap1_skin2'"), 0);
		Character->SetContentMeshMat(Character->Cap_equip, TEXT(" Material'/Game/Characters/Military/Materials/Mat_Military_Cap1_skin1.Mat_Military_Cap1_skin1'"), 0);
		Character->SetContentMeshMat(Character->Head_equip1, nullptr, 0);
		Character->SetContentMeshMat(Character->Head_equip2, nullptr, 0);
		Character->SetContentMeshMat(Character->Shirt, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Shirt5_skin1.Mat_Military_Shirt5_skin1'"), 0);
		Character->SetContentMeshMat(Character->Pants, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Pants1_skin2.Mat_Military_Pants1_skin2'"), 0);
		Character->SetContentMeshMat(Character->Boots, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Boots3_Skin1.Mat_Military_Boots3_Skin1'"), 0);
		Character->SetContentMeshMat(Character->Hands, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Hands1_skin2.Mat_Military_Hands1_skin2'"), 0);
		Character->SetContentMeshMat(Character->Vest, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Vest2_skin2.Mat_Military_Vest2_skin2'"));
		Character->SetContentMeshMat(Character->Vest_outfit, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Vest2_skin2.Mat_Military_Vest2_skin2'"));
		Character->SetContentMeshMat(Character->Vest_outfit, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Equip2_skin3.Mat_Military_Equip2_skin3'"), 1);
		Character->SetContentMeshMat(Character->Vest_outfit, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Pouches1_skin3.Mat_Military_Pouches1_skin3'"), 2);
		Character->SetContentMeshMat(Character->Backpack, nullptr, 0);
		Character->SetContentMeshMat(Character->Backpack2, nullptr, 0);
		Character->SetContentMeshMat(Character->Vest_Collar, nullptr);
		Character->SetContentMeshMat(Character->Vest_shoulder_R, nullptr, 0);
		Character->SetContentMeshMat(Character->Vest_shoulder_L, nullptr, 0);
		Character->SetContentMeshMat(Character->Vest_Bottom, nullptr, 0);
		Character->SetContentMeshMat(Character->Decals, nullptr, 0);
		Character->SetContentMeshMat(Character->Radio, nullptr, 0);
		Character->SetContentMeshMat(Character->Kneepad_R, nullptr, 0);
		Character->SetContentMeshMat(Character->Kneepad_L, nullptr, 0);
		Character->SetContentMeshMat(Character->Holster, nullptr, 0);

		Character->CharacterStat->SetClassData(classNumber);
		Character->SetFXSound(TEXT("SoundWave'/Game/AUDIO/Sound/Voice/HumanMaleD/voice_male_d_hurt_pain_05.voice_male_d_hurt_pain_05'"),
			TEXT("SoundWave'/Game/AUDIO/Sound/Voice/HumanMaleD/voice_male_d_death_09.voice_male_d_death_09'"));
	}
	else if (classNumber == 6)  // Ranger
	{
		Character->SetContentMesh(Character->GetMesh(), TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Body/SK_Military_Head4.SK_Military_Head4'"));
		Character->SetContentMesh(Character->Cap, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Head/SK_Military_Helmet1_for_goggles.SK_Military_Helmet1_for_goggles'"));
		Character->SetContentMesh(Character->Cap_equip, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Head/SK_Military_Helmet1_fastering.SK_Military_Helmet1_fastering'"));
		Character->SetContentMesh(Character->Head_equip1, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Head/SK_Military_Goggles2_Helmet.SK_Military_Goggles2_Helmet'"));
		Character->SetContentMesh(Character->Head_equip2, nullptr);
		Character->SetContentMesh(Character->Shirt, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Shirt4.SK_Military_Shirt4'"));
		Character->SetContentMesh(Character->Pants, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Pants1.SK_Military_Pants1'"));
		Character->SetContentMesh(Character->Boots, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Boots5.SK_Military_Boots5'"));
		Character->SetContentMesh(Character->Hands, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Body/SK_Military_Gloves_Male.SK_Military_Gloves_Male'"));
		Character->SetContentMesh(Character->Vest, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Heavy_Vest.SK_Military_Heavy_Vest'"));
		Character->SetContentMesh(Character->Vest_outfit, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Outfit/SK_Military_Outfit1.SK_Military_Outfit1'"));
		Character->SetContentMesh(Character->Backpack, nullptr);
		Character->SetContentMesh(Character->Backpack2, nullptr);
		Character->SetContentMesh(Character->Vest_Collar, nullptr);
		Character->SetContentMesh(Character->Vest_shoulder_R, nullptr);
		Character->SetContentMesh(Character->Vest_shoulder_L, nullptr);
		Character->SetContentMesh(Character->Vest_Bottom, nullptr);
		Character->SetContentMesh(Character->Decals, nullptr);
		Character->SetContentMesh(Character->Radio, nullptr);
		Character->SetContentMesh(Character->Kneepad_R, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Kneepad_R.SK_Military_Kneepad_R'"));
		Character->SetContentMesh(Character->Kneepad_L, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Kneepad_L.SK_Military_Kneepad_L'"));
		Character->SetContentMesh(Character->Holster, nullptr);

		Character->SetContentMeshMat(Character->GetMesh(), TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Head4.Mat_Military_Head4'"), 0);
		Character->SetContentMeshMat(Character->Cap, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Cap1_skin1.Mat_Military_Cap1_skin1'"), 0);
		Character->SetContentMeshMat(Character->Cap_equip, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Cap1_skin1.Mat_Military_Cap1_skin1'"), 0);
		Character->SetContentMeshMat(Character->Head_equip1, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Cap1_skin1.Mat_Military_Cap1_skin1'"), 0);
		Character->SetContentMeshMat(Character->Head_equip1, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Glass.Mat_Military_Glass'"), 1);
		Character->SetContentMeshMat(Character->Head_equip2, nullptr, 0);
		Character->SetContentMeshMat(Character->Shirt, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Shirt4_skin4.Mat_Military_Shirt4_skin4'"), 0);
		Character->SetContentMeshMat(Character->Pants, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Pants1_skin3.Mat_Military_Pants1_skin3'"), 0);
		Character->SetContentMeshMat(Character->Boots, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Boots4-5_skin1.Mat_Military_Boots4-5_skin1'"), 0);
		Character->SetContentMeshMat(Character->Hands, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Hands1_skin1.Mat_Military_Hands1_skin1'"), 0);
		Character->SetContentMeshMat(Character->Hands, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Equip2_skin1.Mat_Military_Equip2_skin1'"), 1);
		Character->SetContentMeshMat(Character->Vest, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Vest2_skin5.Mat_Military_Vest2_skin5'"));
		Character->SetContentMeshMat(Character->Vest_outfit, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Vest1_skin1.Mat_Military_Vest1_skin1'"));
		Character->SetContentMeshMat(Character->Vest_outfit, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Equip2_skin3.Mat_Military_Equip2_skin3'"), 1);
		Character->SetContentMeshMat(Character->Vest_outfit, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Pouches1_skin1.Mat_Military_Pouches1_skin1'"), 2);
		Character->SetContentMeshMat(Character->Backpack, nullptr, 0);
		Character->SetContentMeshMat(Character->Backpack2, nullptr, 0);
		Character->SetContentMeshMat(Character->Vest_Collar, nullptr);
		Character->SetContentMeshMat(Character->Vest_shoulder_R, nullptr, 0);
		Character->SetContentMeshMat(Character->Vest_shoulder_L, nullptr, 0);
		Character->SetContentMeshMat(Character->Vest_Bottom, nullptr, 0);
		Character->SetContentMeshMat(Character->Decals, nullptr, 0);
		Character->SetContentMeshMat(Character->Radio, nullptr, 0);
		Character->SetContentMeshMat(Character->Kneepad_R, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Pants1_skin1.Mat_Military_Pants1_skin1'"), 0);
		Character->SetContentMeshMat(Character->Kneepad_L, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Pants1_skin1.Mat_Military_Pants1_skin1'"), 0);
		Character->SetContentMeshMat(Character->Holster, nullptr, 0);

		Character->CharacterStat->SetClassData(classNumber);
	}
	else if (classNumber == 7) //sniper
	{
		Character->SetContentMesh(Character->GetMesh(), TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Body/SK_Military_Head5.SK_Military_Head5'"));
		Character->SetContentMesh(Character->Cap, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Head/SK_Military_Helmet2.SK_Military_Helmet2'"));
		Character->SetContentMesh(Character->Cap_equip, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Head/SK_Military_Helmet2_fastering.SK_Military_Helmet2_fastering'"));
		Character->SetContentMesh(Character->Head_equip1, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Head/SK_Military_NVG_disabled.SK_Military_NVG_disabled'"));
		Character->SetContentMesh(Character->Head_equip2, nullptr);
		Character->SetContentMesh(Character->Shirt, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Shirt5.SK_Military_Shirt5'"));
		Character->SetContentMesh(Character->Pants, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Pants5.SK_Military_Pants5'"));
		Character->SetContentMesh(Character->Boots, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Boots3.SK_Military_Boots3'"));
		Character->SetContentMesh(Character->Hands, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Body/SK_Military_Hands_Male.SK_Military_Hands_Male'"));
		Character->SetContentMesh(Character->Vest, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Vest1.SK_Military_Vest1'"));
		Character->SetContentMesh(Character->Vest_outfit, nullptr);
		Character->SetContentMesh(Character->Backpack, nullptr);
		Character->SetContentMesh(Character->Backpack2, nullptr);
		Character->SetContentMesh(Character->Vest_Collar, nullptr);
		Character->SetContentMesh(Character->Vest_shoulder_R, nullptr);
		Character->SetContentMesh(Character->Vest_shoulder_L, nullptr);
		Character->SetContentMesh(Character->Vest_Bottom, nullptr);
		Character->SetContentMesh(Character->Decals, nullptr);
		Character->SetContentMesh(Character->Radio, nullptr);
		Character->SetContentMesh(Character->Kneepad_R, nullptr);
		Character->SetContentMesh(Character->Kneepad_L, nullptr);
		Character->SetContentMesh(Character->Holster, nullptr);

		Character->SetContentMeshMat(Character->GetMesh(), TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Head5.Mat_Military_Head5'"), 0);
		Character->SetContentMeshMat(Character->Cap, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Helmet2_skin2.Mat_Military_Helmet2_skin2'"), 0);
		Character->SetContentMeshMat(Character->Cap_equip, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Helmet2_skin1.Mat_Military_Helmet2_skin1'"), 0);
		Character->SetContentMeshMat(Character->Head_equip1, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Helmet2_skin1.Mat_Military_Helmet2_skin1'"), 0);
		Character->SetContentMeshMat(Character->Head_equip2, nullptr, 0);
		Character->SetContentMeshMat(Character->Shirt, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Shirt5_skin3.Mat_Military_Shirt5_skin3'"), 0);
		Character->SetContentMeshMat(Character->Pants, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Pants5_skin1.Mat_Military_Pants5_skin1'"), 0);
		Character->SetContentMeshMat(Character->Boots, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Boots3_Skin1.Mat_Military_Boots3_Skin1'"), 0);
		Character->SetContentMeshMat(Character->Hands, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Hands1_skin1.Mat_Military_Hands1_skin1'"), 0);
		Character->SetContentMeshMat(Character->Vest, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Vest1_skin3.Mat_Military_Vest1_skin3'"));
		Character->SetContentMeshMat(Character->Vest_outfit, nullptr);
		Character->SetContentMeshMat(Character->Backpack, nullptr, 0);
		Character->SetContentMeshMat(Character->Backpack2, nullptr, 0);
		Character->SetContentMeshMat(Character->Vest_Collar, nullptr);
		Character->SetContentMeshMat(Character->Vest_shoulder_R, nullptr, 0);
		Character->SetContentMeshMat(Character->Vest_shoulder_L, nullptr, 0);
		Character->SetContentMeshMat(Character->Vest_Bottom, nullptr, 0);
		Character->SetContentMeshMat(Character->Decals, nullptr, 0);
		Character->SetContentMeshMat(Character->Radio, nullptr, 0);
		Character->SetContentMeshMat(Character->Kneepad_R, nullptr, 0);
		Character->SetContentMeshMat(Character->Kneepad_L, nullptr, 0);
		Character->SetContentMeshMat(Character->Holster, nullptr, 0);

		Character->CharacterStat->SetClassData(classNumber);
		Character->SetFXSound(TEXT("SoundWave'/Game/AUDIO/Sound/Voice/HumanMaleC/voice_male_c_hurt_pain_04.voice_male_c_hurt_pain_04'"),
			TEXT("SoundWave'/Game/AUDIO/Sound/Voice/HumanMaleC/voice_male_c_death_06.voice_male_c_death_06'"));
	}
	else if (classNumber == 8) // scout
	{
		Character->SetContentMesh(Character->GetMesh(), TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Body/SK_Military_Head3.SK_Military_Head3'"));
		Character->SetContentMesh(Character->Cap, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Head/SK_Military_Cap4.SK_Military_Cap4'"));
		Character->SetContentMesh(Character->Cap_equip, nullptr);
		Character->SetContentMesh(Character->Head_equip1, nullptr);
		Character->SetContentMesh(Character->Head_equip2, nullptr);
		Character->SetContentMesh(Character->Shirt, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Shirt4.SK_Military_Shirt4'"));
		Character->SetContentMesh(Character->Pants, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Pants4.SK_Military_Pants4'"));
		Character->SetContentMesh(Character->Boots, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/SK_Military_Boots3.SK_Military_Boots3'"));
		Character->SetContentMesh(Character->Hands, TEXT("SkeletalMesh'/Game/Characters/Military/Mesh/Body/SK_Military_Hands_Male.SK_Military_Hands_Male'"));
		Character->SetContentMesh(Character->Vest, nullptr);
		Character->SetContentMesh(Character->Vest_outfit, nullptr);
		Character->SetContentMesh(Character->Backpack, nullptr);
		Character->SetContentMesh(Character->Backpack2, nullptr);
		Character->SetContentMesh(Character->Vest_Collar, nullptr);
		Character->SetContentMesh(Character->Vest_shoulder_R, nullptr);
		Character->SetContentMesh(Character->Vest_shoulder_L, nullptr);
		Character->SetContentMesh(Character->Vest_Bottom, nullptr);
		Character->SetContentMesh(Character->Decals, nullptr);
		Character->SetContentMesh(Character->Radio, nullptr);
		Character->SetContentMesh(Character->Kneepad_R, nullptr);
		Character->SetContentMesh(Character->Kneepad_L, nullptr);
		Character->SetContentMesh(Character->Holster, nullptr);

		Character->SetContentMeshMat(Character->GetMesh(), TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Head3.Mat_Military_Head3'"), 0);
		Character->SetContentMeshMat(Character->Cap, TEXT("MMaterial'/Game/Characters/Military/Materials/Mat_Military_Cap4_skin2.Mat_Military_Cap4_skin2'"), 0);
		Character->SetContentMeshMat(Character->Cap_equip, nullptr, 0);
		Character->SetContentMeshMat(Character->Head_equip1, nullptr, 0);
		Character->SetContentMeshMat(Character->Head_equip2, nullptr, 0);
		Character->SetContentMeshMat(Character->Shirt, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Shirt4_skin1.Mat_Military_Shirt4_skin1'"), 0);
		Character->SetContentMeshMat(Character->Pants, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Pants4_skin1.Mat_Military_Pants4_skin1'"), 0);
		Character->SetContentMeshMat(Character->Boots, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Boots3_Skin1.Mat_Military_Boots3_Skin1'"), 0);
		Character->SetContentMeshMat(Character->Hands, TEXT("Material'/Game/Characters/Military/Materials/Mat_Military_Hands2_skin2.Mat_Military_Hands2_skin2'"), 0);
		Character->SetContentMeshMat(Character->Vest, nullptr);
		Character->SetContentMeshMat(Character->Vest_outfit, nullptr);
		Character->SetContentMeshMat(Character->Backpack, nullptr, 0);
		Character->SetContentMeshMat(Character->Backpack2, nullptr, 0);
		Character->SetContentMeshMat(Character->Vest_Collar, nullptr);
		Character->SetContentMeshMat(Character->Vest_shoulder_R, nullptr, 0);
		Character->SetContentMeshMat(Character->Vest_shoulder_L, nullptr, 0);
		Character->SetContentMeshMat(Character->Vest_Bottom, nullptr, 0);
		Character->SetContentMeshMat(Character->Decals, nullptr, 0);
		Character->SetContentMeshMat(Character->Radio, nullptr, 0);
		Character->SetContentMeshMat(Character->Kneepad_R, nullptr, 0);
		Character->SetContentMeshMat(Character->Kneepad_L, nullptr, 0);
		Character->SetContentMeshMat(Character->Holster, nullptr, 0);

		Character->CharacterStat->SetClassData(classNumber);
	}
	else
	{

	}

}

void ASquadGameMode::BeginPlay()
{
	Super::BeginPlay();

	auto SplayerController = Cast<ASquadController>(GetWorld()->GetFirstPlayerController());
	SplayerController->SetSquadControllerInput(false);
	DisableInput(SplayerController);
	//CheckArrayNum();
}

UUserWidget* ASquadGameMode::GetCurrentWidget()
{
	return CurrentWidget;
}

UUserWidget* ASquadGameMode::GetCurrentSubWidget()
{
	return CurrentSubWidget;
}

void ASquadGameMode::ViewDecisionWidget()
{
	ChangeSubWidget(DecisionWidgetClass);
}

void ASquadGameMode::ViewDecisionBossWidget()
{
	ChangeSubWidget(DecisionBossWidgetClass);
}

void ASquadGameMode::ViewDecisionBossResultWidget(EDecisionEventResultEnum DecisionResult, int32 MainStream, int32 SubStream, int32 SubStreamSuccFail, FText EventResultText, FText EventValueText)
{
	ChangeBossOutputWidget(DecisionBossResultWidgetClass);
}

void ASquadGameMode::ViewDecisionResultWidget(EDecisionEventResultEnum DecisionResult, int32 MainStream, int32 SubStream, int32 SubStreamSuccFail, FText EventResultText, FText EventValueText)
{	
	ChangeOutputWidget(DecisionResultWidgetClass, DecisionResult,MainStream, SubStream, SubStreamSuccFail, EventResultText, EventValueText);
}

void ASquadGameMode::ViewBattleWidget()
{
	ChangeMenuWidget(BattleStartWidgetClass);
}

void ASquadGameMode::ViewVictoryWidget()
{
	ChangeMenuWidget(VictoryWidgetClass);
}

void ASquadGameMode::ViewDefeatWidget()
{
	ChangeMenuWidget(DefeatWidgetClass);
}

TSubclassOf<UUserWidget> ASquadGameMode::GetVictoryWidgetClass()
{
	return VictoryWidgetClass;
}

TSubclassOf<UUserWidget> ASquadGameMode::GetDefeatWidgetClass()
{
	return DefeatWidgetClass;
}

void ASquadGameMode::StartBattle()
{
	Cast<ABattleTrigger>(BTIns)->InitBattleSetting();
	
	//BTIns = nullptr;
	/*
	auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	gameIns->BCIns->InitBattleSetting();
	*/
}

void ASquadGameMode::EndBattle()
{
	CurrentWidget->RemoveFromViewport();
	CurrentWidget = nullptr;
}

void ASquadGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}	
}

void ASquadGameMode::ChangeSubWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentSubWidget != nullptr)
	{
		CurrentSubWidget->RemoveFromViewport();
		CurrentSubWidget = nullptr;
	}

	if (NewWidgetClass != nullptr)
	{
		CurrentSubWidget = CreateWidget(GetWorld(), NewWidgetClass);
		if (CurrentSubWidget != nullptr)
		{
			CurrentSubWidget->AddToViewport();
		}
	}
}

void ASquadGameMode::ChangeOutputWidget(TSubclassOf<UUserWidget> NewWidgetClass, EDecisionEventResultEnum DecisionResult, int32 MainStream, int32 SubStream , int32 SubStreamSuccFail, FText EventResultText, FText EventValueText)
{
	if (CurrentSubWidget != nullptr)
	{
		CurrentSubWidget->RemoveFromViewport();
		CurrentSubWidget = nullptr;
	}

	if (NewWidgetClass != nullptr)
	{
		CurrentSubWidget = CreateWidget(GetWorld(), NewWidgetClass);

		auto Widget = Cast<UDecisionResultWidget>(CurrentSubWidget);
			Widget->DecisionResultEnum = DecisionResult;
			Widget->MainStream = MainStream;
			Widget->SubStream = SubStream;
			Widget->SubStreamSuccFail = SubStreamSuccFail;
			Widget->EventResultText = EventResultText;
			Widget->EventResultValueText = EventValueText;

		if (CurrentSubWidget != nullptr)
		{
			CurrentSubWidget->AddToViewport();
		}
	}
}

void ASquadGameMode::ChangeBossOutputWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentSubWidget != nullptr)
	{
		CurrentSubWidget->RemoveFromViewport();
		CurrentSubWidget = nullptr;
	}

	if (NewWidgetClass != nullptr)
	{
		CurrentSubWidget = CreateWidget(GetWorld(), NewWidgetClass);

		auto Widget = Cast<UDecisionResultWidget>(CurrentSubWidget);
		Widget->DecisionResultEnum = EDecisionEventResultEnum::BossBattleStart;
		Widget->MainStream = 999;


		if (CurrentSubWidget != nullptr)
		{
			CurrentSubWidget->AddToViewport();
		}
	}
}

void ASquadGameMode::ChangeTargetName(FString Name)
{
	Cast<UBattleWidget>(CurrentWidget)->ChangeTargetName(Name);
}

void ASquadGameMode::ChangeCurrentCharName(FString Name)
{
	Cast<UBattleWidget>(CurrentWidget)->ChangeCurrentCharName(Name);
}

void ASquadGameMode::ChangeMaxAmmoWidgetText(float MaxAmmo)
{
	Cast<UBattleWidget>(CurrentWidget)->ChangeMaxAmmoText(MaxAmmo);
}

void ASquadGameMode::ChangeCurrentAmmoWidgetText(float CurrentAmmo)
{
	Cast<UBattleWidget>(CurrentWidget)->ChangeCurrentAmmoText(CurrentAmmo);
}

void ASquadGameMode::ChangeSkillButtonWidgetImage(APlayerSquadCharacter* PlayerCharacter)
{
	Cast<UBattleWidget>(CurrentWidget)->ChangeSkillButtonImage(PlayerCharacter);
}

void ASquadGameMode::ChangeWhosTurnName(bool Name)
{
	Cast<UBattleWidget>(CurrentWidget)->ChangeWhosTurnName(Name);
}

void ASquadGameMode::UpDateWidgetText(ASquadCharacter* Char)
{
	if(Char) {
		FText name = Cast<APlayerSquadCharacter>(Char)->CharacterStat->GetCharacterBrunchName();//Char->GetName();
		float MaxAmmo = Cast<APlayerSquadCharacter>(Char)->CharacterStat->GetWeaponMaxFireCount();
		float CurrentAmmo = Char->CurrentAmmo;
		float MaxLifePoint = Char->MaxLifePoint;
		float CurrentLifePoint = Char->LifePoint;

		Cast<UBattleWidget>(CurrentWidget)->UpdateCharacterInfoText(name, MaxAmmo, CurrentAmmo, MaxLifePoint, CurrentLifePoint);
	}
}

void ASquadGameMode::UpdataWidget_TargetCharacterPanel(ASquadCharacter * TargetChar)
{
	Cast<UBattleWidget>(CurrentWidget)->UpdateWiget_TargetCharacterPanel(TargetChar);
}

void ASquadGameMode::UpdateWidget_TargetCharacterStatPanel(ASquadCharacter* TargetChar, ASquadCharacter* SelectedChar)
{
	Cast<UBattleWidget>(CurrentWidget)->UpdateWiget_TargetCharacterStatPanel(TargetChar, SelectedChar);	
}

void ASquadGameMode::UpDateWidgetDamageGraph(ASquadCharacter* Char)
{
	Cast<UBattleWidget>(CurrentWidget)->UpdateCharacterDamageGraph(Char);
}

void ASquadGameMode::UpDateWidgetDamageGraph_Blank()
{
	Cast<UBattleWidget>(CurrentWidget)->UpdateCharacterDamageGraph_Blank();
}

void ASquadGameMode::UpDateWidgetText_Right2(ASquadCharacter* Char)
{
	APlayerSquadCharacter* PChar = Cast<APlayerSquadCharacter>(Char);

	float Acc = PChar->CharacterStat->GetCharacterAccuracyCorrectionValue() + PChar->CharacterStat->GetWeaponAccuracy();
	float Crit = PChar->CharacterStat->GetWeaponCritical() + PChar->CharacterStat->GetCharacterCriticalCorrectionValue();
	float Damage = PChar->CharacterStat->GetWeaponDamage();
	float HitCount = PChar->CharacterStat->GetWeaponFireCount();
	float MaxDamage = (Damage * HitCount);
	float Evasion = PChar->CharacterStat->GetWeaponAvoidence() + PChar->CharacterStat->GetCharacterEvasionCorrectionValue() + PChar->Evasion;
	float Defense = PChar->Defense;

	Cast<UBattleWidget>(CurrentWidget)->UpdateCharacterInfoText_Right2(Acc, Crit, Damage, HitCount, MaxDamage, Evasion, Defense, Char);
}

void ASquadGameMode::UpDataWidgetText_Skill(APlayerSquadCharacter* Char, int32 skillNumber)
{
	Cast<UBattleWidget>(CurrentWidget)->Updata_CharacterSkill_Info(Char, skillNumber);
}

void ASquadGameMode::ClearCharacterInfoWidgetText()
{
	Cast<UBattleWidget>(CurrentWidget)->ClearCharacterInfoText_Left();
	Cast<UBattleWidget>(CurrentWidget)->ClearCharacterInfoText_Right();
}

void ASquadGameMode::ClearCharacterInfoWidgetText_Right()
{
	Cast<UBattleWidget>(CurrentWidget)->ClearCharacterInfoText_Right();
}

void ASquadGameMode::UpDateWidget_SkillPanel(class APlayerSquadCharacter* PCharacter, float SkillNum)
{
	Cast<UBattleWidget>(CurrentWidget)->UpdateWidget_SkillPart(PCharacter, SkillNum);
}

void ASquadGameMode::ClearWidget_SkillPanel()
{
	Cast<UBattleWidget>(CurrentWidget)->ClearWidget_SkillPart();
}

void ASquadGameMode::Set_BattleWidgetOpacity(float val)
{
	Cast<UBattleWidget>(CurrentWidget)->Set_BattleWidgetOpacity(val);
}

void ASquadGameMode::LoadCharacterMesh()
{

}

void ASquadGameMode::ViewGameVictoryWidget()
{
	Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->IsBattleStart = false;
	ChangeMenuWidget(GameVictoryWidgetClass);
}

void ASquadGameMode::SetBattleWidgetSkillButtonOpacity_transparent()
{
	Cast<UBattleWidget>(CurrentWidget)->Set_BattleWidgetSkilliconOpacity(false);
}
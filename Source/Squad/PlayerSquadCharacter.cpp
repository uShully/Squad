// Fill out your copyright notice in the Description page of Project Settings.

#define BuffOn  true
#define BuffOff false

#include "PlayerSquadCharacter.h"
#include "CharacterAnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "SquadGameInstance.h"
#include "Grid.h"
#include "SquadCharacterStatComponent.h"
#include "Engine/Engine.h"
#include "SquadGameMode.h"
#include "SquadCharacterSkillComponent.h"
#include "SquadController.h"
#include "SquadAIController.h"
#include "BattleWidget.h"
//#include "Kismet/GameplayStatics.h"
//#include "UObject/ConstructorHelpers.h"

APlayerSquadCharacter::APlayerSquadCharacter()
{
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		PlayerCharacterAnimBP(TEXT("Blueprint'/Game/BLUEPRINT/PlayerCharacterAnimInstance.PlayerCharacterAnimInstance_C'"));
	if (PlayerCharacterAnimBP.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(PlayerCharacterAnimBP.Class);
		UE_LOG(LogClass, Log, TEXT(" Anim Succeeded "));
	}
	/*
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("GUNSOUND"));
	AudioComp->SetupAttachment(RootComponent);
	*/

	/*
	static ConstructorHelpers::FObjectFinder<USoundBase> FireSound(L"SoundWave'/Game/AUDIO/Sound/GunSound/gun_rifle_shot_01.gun_rifle_shot_01'");
	if (FireSound.Succeeded())
	{
		Fire_Sound = FireSound.Object;
	}
	*/
	//

	static ConstructorHelpers::FObjectFinder<USoundBase> GetHitSound(L"SoundWave'/Game/AUDIO/HumanMaleA/Wavs/voice_male_grunt_pain_12.voice_male_grunt_pain_12'");
	if (GetHitSound.Succeeded())
	{
		GetHit_Sound = GetHitSound.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> DeathSound(L"SoundWave'/Game/AUDIO/HumanMaleA/Wavs/voice_male_grunt_pain_death_08.voice_male_grunt_pain_death_08'");
	if (DeathSound.Succeeded())
	{
		Death_Sound = DeathSound.Object;
	}

	//

	static ConstructorHelpers::FObjectFinder<USoundBase> SelectedSound(L"SoundWave'/Game/AUDIO/SFX/UI_Button.UI_Button'");
	if (SelectedSound.Succeeded())
	{
		Selected_Sound = SelectedSound.Object;
	}
	

	Cap = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Cap"));
	Cap_equip = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Cap equip"));
	Head_equip1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head equip1"));
	Head_equip2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head equip2"));
	Shirt = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shirt"));
	Pants = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pants"));
	Boots = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Boots"));
	Hands = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hands"));
	Vest = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Vest"));
	Vest_outfit = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Vest outfit"));
	Backpack = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Backpac"));
	Backpack2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Backpac2"));
	Vest_Collar = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VestCollar"));
	Vest_shoulder_R = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Vest shoulder R"));
	Vest_shoulder_L = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Vest shoulder L"));
	Vest_Bottom = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Vest Bottom"));
	Decals = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Decals"));
	Radio = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Radio"));
	Kneepad_R = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Kneepad R"));
	Kneepad_L = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Kneepad L"));
	Holster = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Holster"));

	Cap->SetupAttachment(GetMesh()); 
	Cap_equip->SetupAttachment(GetMesh());
	Head_equip1->SetupAttachment(GetMesh());
	Head_equip2->SetupAttachment(GetMesh());
	Shirt->SetupAttachment(GetMesh());
	Pants->SetupAttachment(GetMesh());
	Boots->SetupAttachment(GetMesh());
	Hands->SetupAttachment(GetMesh());
	Vest->SetupAttachment(GetMesh());
	Vest_outfit->SetupAttachment(GetMesh());
	Backpack->SetupAttachment(GetMesh());
	Backpack2->SetupAttachment(GetMesh());
	Vest_Collar->SetupAttachment(GetMesh());
	Vest_shoulder_R->SetupAttachment(GetMesh());
	Vest_shoulder_L->SetupAttachment(GetMesh());
	Vest_Bottom->SetupAttachment(GetMesh());
	Decals->SetupAttachment(GetMesh());
	Radio->SetupAttachment(GetMesh());
	Kneepad_R->SetupAttachment(GetMesh());
	Kneepad_L->SetupAttachment(GetMesh());
	Holster->SetupAttachment(GetMesh());

	// c++ 사용시 Parent Socket을 지정하지 못하는 문제를 해결하기위해서
	// AttachTo를 사용해서 지정해야한다.
	WeaponSlot = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSlot"));
	//WeaponSlot->AttachTo(Hands, TEXT("weaponSocket"), EAttachLocation::KeepRelativeOffset, true);
	WeaponSlot->AttachToComponent(Hands, FAttachmentTransformRules::KeepRelativeTransform, TEXT("weaponSocket"));
		
	LifeBar->SetWorldLocation(FVector(50.f, -50.f, 200.f));

	CharacterSkillComp = CreateDefaultSubobject<USquadCharacterSkillComponent>(TEXT("SKillComp"));

	
}

void APlayerSquadCharacter::BeginPlay()
{
	Super::BeginPlay();


	//Calc_Damage_distribution();
	//Cast<UStatusBarWidget>(LifeBar->GetUserWidgetObject())->SetProgressBarImage(2);
}

void APlayerSquadCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(Character_BattleRotator_Start == true && IsCharacterUseAttack == false) {
		SetActorRotation(FMath::Lerp(GetActorRotation(), Character_Rotator_StartRotator, DeltaTime*10));
		if (GetActorRotation() == Character_Rotator_StartRotator)
			Character_BattleRotator_Start = false;
	}
	if (IsCharacterUseAttack == true) {
		FVector CharLoc = GetActorLocation();
		FVector TargetLoc = tempTargetCharacter->GetActorLocation();
		FVector Rot = TargetLoc - CharLoc;
		auto InRot = FMath::RInterpTo(GetActorRotation(), Rot.ToOrientationRotator(), DeltaTime, 10.f);
		SetActorRotation(InRot);
	}


	if (Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->IsBattleStart == true
		&& Cast<ASquadAIController>(GetController())->GetMoveStatus() == EPathFollowingStatus::Idle
		&& Cast<UCharacterAnimInstance>(animInstance)->IsSpreadOut == true) {
		Cast<UCharacterAnimInstance>(animInstance)->IsSpreadOut = false;
		Cast<UCharacterAnimInstance>(animInstance)->Call_GetIsSpreadOut();
	}
}

void APlayerSquadCharacter::InitCharacterStat()
{
	LifePoint = CharacterStat->GetCharacterHPValue();
	MaxLifePoint = LifePoint;
	CurrentAmmo = CharacterStat->GetWeaponMaxFireCount();
	MaxAmmo = CurrentAmmo;

	GetStatustBarWidget()->GetHealthBar()->PercentDelegate.BindUFunction(this, FName("GetLifePointPercent"));
	GetStatustBarWidget()->GetAmmoBar()->PercentDelegate.BindUFunction(this, FName("GetAmmoPercent"));

	SetPlayerSkill_ClassNum(ClassNum);
}

void APlayerSquadCharacter::SetContentMesh(USkeletalMeshComponent* mesh, const TCHAR* ContentPath)
{
	if (ContentPath == nullptr)
	{
		mesh = nullptr;
	}
	else if(ContentPath != nullptr)
	{
		USkeletalMesh* NewMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), NULL, ContentPath));
		if (NewMesh)
		{
			mesh->SetSkeletalMesh(NewMesh);	
		}
	}
	
}

void APlayerSquadCharacter::SetFXSound(const TCHAR* HitSoundContentPath, const TCHAR* DeadSoundContentPath)
{
	if (HitSoundContentPath != nullptr)
	{
		USoundBase* NewSound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), NULL, HitSoundContentPath));
		if (NewSound)
		{
			GetHit_Sound = NewSound;
		}
	}
	else
	{
		GetHit_Sound = nullptr;
	}

	if (DeadSoundContentPath != nullptr)
	{
		USoundBase* NewSound_2 = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), NULL, DeadSoundContentPath));
		if (NewSound_2)
		{
			Death_Sound = NewSound_2;
		}
	}
	else
	{
		Death_Sound = nullptr;
	}

}

void APlayerSquadCharacter::SetContentMeshMat(USkeletalMeshComponent* mesh, const TCHAR* ContentPath ,int32 MatIndex)
{
	
	if (ContentPath != nullptr)
	{		
		UMaterial* MeshMat = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, ContentPath));
		if (MeshMat)
		{
			mesh->SetMaterial(MatIndex ,MeshMat);
		
		}
		else
		{
		
		}
	}
	else
	{
		mesh = nullptr;
	}
}



void APlayerSquadCharacter::SetWeaponMesh()  //USkeletalMeshComponent* mesh
{
	FName WeaponSocket(TEXT("weaponSocket"));
	if (Hands->DoesSocketExist(WeaponSocket))
	{		
		AttachToComponent(Hands, FAttachmentTransformRules::KeepRelativeTransform, "weaponSocket");
	
	}
}

void APlayerSquadCharacter::SetSkeletalMeshComp(USkeletalMesh* Head, USkeletalMesh* Cap, USkeletalMesh* Cap_equip
	, USkeletalMesh* Head_equip1, USkeletalMesh* Head_equip2, USkeletalMesh* Shirt
	, USkeletalMesh* Pants, USkeletalMesh* Boots, USkeletalMesh* Hands, USkeletalMesh* Vest
	, USkeletalMesh* Vest_outfit, USkeletalMesh* Backpack, USkeletalMesh* Backpack2, USkeletalMesh* Vest_collar
	, USkeletalMesh* Vest_shoulder_R, USkeletalMesh* Vest_shoulder_L, USkeletalMesh* Vest_bottom, USkeletalMesh* Decals
	, USkeletalMesh* Radio, USkeletalMesh* Kneepad_R, USkeletalMesh* Kneepad_L, USkeletalMesh* Holster)
{
	if (Head != nullptr) GetMesh()->SetSkeletalMesh(Head);
	if (Cap != nullptr) this->Cap->SetSkeletalMesh(Cap);
	if (Cap_equip != nullptr) this->Cap_equip->SetSkeletalMesh(Cap_equip);
	if (Head_equip1 != nullptr) this->Head_equip1->SetSkeletalMesh(Head_equip1);
	if (Head_equip2 != nullptr) this->Head_equip2->SetSkeletalMesh(Head_equip2);
	if (Shirt != nullptr) this->Shirt->SetSkeletalMesh(Shirt);
	if (Pants != nullptr) this->Pants->SetSkeletalMesh(Pants);
	if (Boots != nullptr) this->Boots->SetSkeletalMesh(Boots);
	if (Hands != nullptr) this->Hands->SetSkeletalMesh(Hands);
	if (Vest != nullptr) this->Vest->SetSkeletalMesh(Vest);
	if (Vest_outfit != nullptr) this->Vest_outfit->SetSkeletalMesh(Vest_outfit);
	if (Backpack != nullptr) this->Backpack->SetSkeletalMesh(Backpack);
	if (Backpack2 != nullptr) this->Backpack2->SetSkeletalMesh(Backpack2);
	if (Vest_collar != nullptr) this->Vest_Collar->SetSkeletalMesh(Vest_collar);
	if (Vest_shoulder_R != nullptr) this->Vest_shoulder_R->SetSkeletalMesh(Vest_shoulder_R);
	if (Vest_shoulder_L != nullptr) this->Vest_shoulder_L->SetSkeletalMesh(Vest_shoulder_L);
	if (Vest_bottom != nullptr) this->Vest_Bottom->SetSkeletalMesh(Vest_bottom);
	if (Decals != nullptr) this->Decals->SetSkeletalMesh(Decals);
	if (Radio != nullptr) this->Radio->SetSkeletalMesh(Radio);
	if (Kneepad_R != nullptr) this->Kneepad_R->SetSkeletalMesh(Kneepad_R);
	if (Kneepad_L != nullptr) this->Kneepad_L->SetSkeletalMesh(Kneepad_L);
	if (Holster != nullptr) this->Holster->SetSkeletalMesh(Holster);
}


////////////////////////////// Shot //////////////////////////////////////////////

void APlayerSquadCharacter::SetShotReady()
{
	if (Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->IsBattleStart != false && (StateEnum != EStateEnum::SE_End && StateEnum != EStateEnum::SE_Death))	{
		
		if(StateEnum == EStateEnum::SE_Shot || StateEnum == EStateEnum::SE_Skill1 || StateEnum == EStateEnum::SE_Skill2)
			Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->SetDisableSkillTargeting(true);
		else if (StateEnum == EStateEnum::SE_Cover || StateEnum == EStateEnum::SE_Reload)
			Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->SetDisableSkillTargeting(false);

		SetHighLight(true);

		StateEnum = EStateEnum::SE_Shot;
		Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->SetSkillTargeting(true);
	}
	Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->UpDateWidget_SkillPanel(this, 0);
	UBattleWidget* BW = Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget());
	BW->ChangeSelectedButton(BW->GetAttackButton());
	UGameplayStatics::PlaySoundAtLocation(this, Selected_Sound, GetActorLocation(), 1.0f);
		
	//DisableInput(Cast<ASquadController>(GetWorld()->GetFirstPlayerController()));
	
}

void APlayerSquadCharacter::Debug_Shot(ASquadCharacter* Target) // 시즌 2에서 함수명 고쳐야함
{
	tempTargetCharacter = Target;
	UCharacterAnimInstance* CharAnimInst = Cast<UCharacterAnimInstance>(animInstance);
	FString WeaponName = CharacterStat->GetCharacterWeaponName();

	Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->Set_BattleWidgetOpacity(0.5f);
	Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->Set_BattleWidgetSkilliconOpacity(false);
	Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->SetDisableSkillTargeting(true);

	if (CharAnimInst != nullptr)
	{
		/*
		if (WeaponName == "Pistol") {
						
				CharAnimInst->AimingPistol();
		;
		}
		else 
		*/
		IsCharacterUseAttack = true;
	

		Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->Set_BattleWidgetSkillButtonActive(false);
		CharAnimInst->Aiming();
	}

	
}

void APlayerSquadCharacter::BeShot()
{
	auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

	FVector CharLoc = GetActorLocation();
	FVector TargetLoc = tempTargetCharacter->GetActorLocation();
	FVector Rot = TargetLoc - CharLoc;
	//SetActorRotation(Rot.ToOrientationRotator());
	
	float HitCount = 0.f;
	float CriticalCount = 0.f;
	float ActualDamage = 0.f;


	if (gameIns->TargetCharacter != nullptr)
	{
		UCharacterAnimInstance* CharAnim = Cast<UCharacterAnimInstance>(animInstance);

		
		//GameStatic->SpawnEmitterAttached(FireParticle, Weapon, FName("MuzzleFlash"));

		FString WeaponName = CharacterStat->GetCharacterWeaponName();

		if (WeaponName == "Rifle") {
			//UGameplayStatics::PlaySoundAtLocation(this, Rifle_Shot_Sound, GetActorLocation(), 0.2f);
			CharAnim->BeShotRifle();
		}
		else if (WeaponName == "Pistol") {
			//UGameplayStatics::PlaySoundAtLocation(this, Pistol_Shot_Sound, GetActorLocation(), 0.2f);
			CharAnim->BeShotPistol();
		}
		else if (WeaponName == "Shotgun") {
			//UGameplayStatics::PlaySoundAtLocation(this, Shotgun_Shot_Sound, GetActorLocation(), 0.2f);
			CharAnim->BeShotShotgun();
		}
		else if (WeaponName == "Sniper") {
			//UGameplayStatics::PlaySoundAtLocation(this, Sniper_Shot_Sound, GetActorLocation(), 0.2f);
			CharAnim->BeShotSniper();
		}

		// 데미지 공식
		// 명중 계산 = (명중) - (회피)
		// 전탄 공격 = (공격) * (탄환)
		// 기대 공격 = (공격) * (탄환) * {(명중) - (회피)} = (전탄공격) * (명중계산)

		// 전탄 치명 = (치명) * (탄환)
		// 기대 치명 = (치명) * (탄환) * {(명중) - (회피)}

		// 맞춘 탄환에 수에 따라서 전탄치명 공식을 대입을 시켜서 적용

		// 예시) 6발 명중 50
		//       3발 적중
		//       3 * 치명타 확률 (5%)
		//       85퍼 확률 일반공격 // 15퍼 확률 치명타

		//  공격 함수에서 데미지를 계산을해서 호출
		//  - 연출하려면 애니메이션 모션이 필요함(주류) - 할순있음 문제가 직접하던가 사던가?
		//  -  

		//           = 연출 =
		//  - 총기화염이 6번이 튀도록 할수있음( 필요가 없음 )
		//  - 총이 발사되는 타타탕 사운드로 표현
		//  - 총알이 날라가는게 없잖(나중에 추가가 될순있어도 현재는 없음) = (이거는 아마 코드로 해결이 가능하긴함)

		// 애니메이션 연출을 그렇하던가 - 함수호출을 해가지고 연출을 그렇게하던가
		// 3 / 3 (제일 편함 애니메이션) 문제 : 애니메이션 모션을 구할수가 없음

		for (int32 i = 0; i < CharacterStat->GetWeaponFireCount(); i++)
		{
			if (FMath::FRandRange(0.f, 100.f) < CharacterStat->GetCharacterAccuracyCorrectionValue() + CharacterStat->GetWeaponAccuracy() - tempTargetCharacter->Evasion) // 맞췃을때
			{
				HitCount++; // 히트 카운트 계산
			}
		}
		for (int32 i = 0; i < HitCount; i++)
		{
			if (FMath::FRandRange(0.f, 100.f) < CharacterStat->GetCharacterCriticalCorrectionValue() + CharacterStat->GetWeaponCritical())
			{
				// 치명타 계산
				CriticalCount++;
			}
		}

		ActualDamage = CharacterStat->GetWeaponDamage() * HitCount + 1000.f * CriticalCount; // 마지막 데미지

		if(ActualDamage > 0)
			UGameplayStatics::ApplyDamage(tempTargetCharacter, ActualDamage, GetWorld()->GetFirstPlayerController(), this, nullptr);
		else if (ActualDamage <= 0) {
			FDamageEvent DamegeEvent;
			Cast<ASquadCharacter>(tempTargetCharacter)->TakeDamage(0, DamegeEvent, nullptr, this);
		}
	}

	if (CurrentAmmo >= 0)
		CurrentAmmo -= 1;

	//SetCharacterEnd();

	if (UnderGrid != nullptr)
		UnderGrid->SetGridInfo_Material_Black();

	//SetIsCharacterUseAttackTotrue();

}

//////////////////////////// Reload /////////////////////////////////////////

void APlayerSquadCharacter::SetReloadReady()
{
	if(CurrentAmmo != MaxAmmo) {
		if (Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->IsBattleStart != false && (StateEnum != EStateEnum::SE_End && StateEnum != EStateEnum::SE_Death)) {

			StateEnum = EStateEnum::SE_Reload;
			Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->SetSkillTargeting(false);
		}
		Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->UpDateWidget_SkillPanel(this, 3);
		UBattleWidget* BW = Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget());
		BW->ChangeSelectedButton(BW->GetReloadButton());
		UGameplayStatics::PlaySoundAtLocation(this, Selected_Sound, GetActorLocation(), 1.0f);
	}
	else {
		if (Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->IsBattleStart != false && (StateEnum != EStateEnum::SE_End && StateEnum != EStateEnum::SE_Death)) {

			StateEnum = EStateEnum::SE_Stay;
		}
		Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->UpDateWidget_SkillPanel(this, 3);
		UBattleWidget* BW = Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget());
		BW->ChangeSelectedButton(BW->GetReloadButton());
		UGameplayStatics::PlaySoundAtLocation(this, Selected_Sound, GetActorLocation(), 1.0f);
	}
}

void APlayerSquadCharacter::BeReload()
{
	auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	UCharacterAnimInstance* CharAnimInst = Cast<UCharacterAnimInstance>(animInstance);

	if ((StateEnum != EStateEnum::SE_End && StateEnum != EStateEnum::SE_Death)) {
		CurrentAmmo = CharacterStat->GetWeaponMaxFireCount();

		FString WeaponName = CharacterStat->GetCharacterWeaponName();


		auto SplayerController = Cast<ASquadController>(GetWorld()->GetFirstPlayerController());

		SplayerController->SetSquadControllerInput(false);
		DisableInput(SplayerController);

		SetHighLight(false);

		Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->Set_BattleWidgetSkillButtonActive(false);
		CharAnimInst->Reload_Crouch();
		/*
		if (WeaponName == "Rifle") {
			UGameplayStatics::PlaySoundAtLocation(this, Rifle_Reload_Sound, GetActorLocation(), 0.2f);
			CharAnimInst->Reload_Rifle();
		}
		else if (WeaponName == "Pistol") {
			UGameplayStatics::PlaySoundAtLocation(this, Pistol_Reload_Sound, GetActorLocation(), 0.2f);
			CharAnimInst->Reload_Pistol();
		}
		else if (WeaponName == "Shotgun") {
			UGameplayStatics::PlaySoundAtLocation(this, Shotgun_Reload_Sound, GetActorLocation(), 0.2f);
			CharAnimInst->Reload_Shotgun();
		}
		else if (WeaponName == "Sniper") {
			UGameplayStatics::PlaySoundAtLocation(this, Sniper_Reload_Sound, GetActorLocation(), 0.2f);
			CharAnimInst->Reload_Sniper();
		}
		*/
		//SetCharacterEnd();

		if (UnderGrid != nullptr)
			UnderGrid->SetGridInfo_Material_Black();
	}
	//gameIns->BCIns->EndTurnSystem();
}

void APlayerSquadCharacter::BeReload_BattleOver()
{
	auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	UCharacterAnimInstance* CharAnimInst = Cast<UCharacterAnimInstance>(animInstance);
	CurrentAmmo = CharacterStat->GetWeaponMaxFireCount();

	FString WeaponName = CharacterStat->GetCharacterWeaponName();

	if (WeaponName == "Rifle") {
		UGameplayStatics::PlaySoundAtLocation(this, Rifle_Reload_Sound, GetActorLocation(), 0.2f);
		//CharAnimInst->Reload_Rifle();
	}
	else if (WeaponName == "Pistol") {
		UGameplayStatics::PlaySoundAtLocation(this, Pistol_Reload_Sound, GetActorLocation(), 0.2f);
		//CharAnimInst->Reload_Pistol();
	}
	else if (WeaponName == "Shotgun") {
		UGameplayStatics::PlaySoundAtLocation(this, Shotgun_Reload_Sound, GetActorLocation(), 0.2f);
		//CharAnimInst->Reload_Shotgun();
	}
	else if (WeaponName == "Sniper") {
		UGameplayStatics::PlaySoundAtLocation(this, Sniper_Reload_Sound, GetActorLocation(), 0.2f);
		//CharAnimInst->Reload_Sniper();
	}

	//SetCharacterEnd();

	if (UnderGrid != nullptr)
		UnderGrid->SetGridInfo_Material_Black();

	//gameIns->BCIns->EndTurnSystem();
}

////////////////////////////// Move //////////////////////////////////////////////

void APlayerSquadCharacter::SetMoveReady()
{
	//StateEnum = EStateEnum::SE_Move;


}

////////////////////////////// Cover //////////////////////////////////////////////

void APlayerSquadCharacter::SetCoverReady()
{
		if (Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->IsBattleStart != false && (StateEnum != EStateEnum::SE_End && StateEnum != EStateEnum::SE_Death)) {

			if (StateEnum == EStateEnum::SE_Shot || StateEnum == EStateEnum::SE_Skill1 || StateEnum == EStateEnum::SE_Skill2)
				Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->SetDisableSkillTargeting(true);
			else if (StateEnum == EStateEnum::SE_Cover || StateEnum == EStateEnum::SE_Reload)
				Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->SetDisableSkillTargeting(false);

			StateEnum = EStateEnum::SE_Cover;
			Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->SetSkillTargeting(false);
		}
		Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->UpDateWidget_SkillPanel(this, 4);
		UBattleWidget* BW = Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget());
		BW->ChangeSelectedButton(BW->GetCoverButton());
		UGameplayStatics::PlaySoundAtLocation(this, Selected_Sound, GetActorLocation(), 1.0f);
}

void APlayerSquadCharacter::SetCover()
{
	if (Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->IsBattleStart != false) {
		if((StateEnum != EStateEnum::SE_End && StateEnum != EStateEnum::SE_Death))
		{
			UCharacterAnimInstance* CharAnim = Cast<UCharacterAnimInstance>(animInstance);
			auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
					
			Buff_Cover(BuffOn);

			SetHighLight(false);
			if (UnderGrid != nullptr)
				UnderGrid->SetGridInfo_Material_Black();

			SetCharacterEnd();		
		}
	}
}

////////////////////////// Stay  ////////////////////////////////////////

void APlayerSquadCharacter::SetStay()
{
	StateEnum = EStateEnum::SE_Stay;

	
	UCharacterAnimInstance* CharAnim = Cast<UCharacterAnimInstance>(animInstance);
	CharAnim->Montage_Stop(0.5f);

}

////////////////////////// SKill ////////////////////////////////////////

void APlayerSquadCharacter::SetSkill1()
{
	if ((Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->IsBattleStart != false)) {
		if (Character_Skill1_Cooldown == 0) {
			if (Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->IsBattleStart != false && (StateEnum != EStateEnum::SE_End && StateEnum != EStateEnum::SE_Death)) {
				if (StateEnum == EStateEnum::SE_Shot || StateEnum == EStateEnum::SE_Skill1 || StateEnum == EStateEnum::SE_Skill2)
					Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->SetDisableSkillTargeting(true);
				else if (StateEnum == EStateEnum::SE_Cover || StateEnum == EStateEnum::SE_Reload)
					Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->SetDisableSkillTargeting(false);
				
				StateEnum = EStateEnum::SE_Skill1;
				SetHighLight(true);
				Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->SetSkillTargeting(true);
			}
			

			Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->UpDataWidgetText_Skill(this, 1);
			Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->UpDateWidget_SkillPanel(this, 1);
			UBattleWidget* BW = Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget());
			BW->ChangeSelectedButton(BW->GetSkill1Button());
			UGameplayStatics::PlaySoundAtLocation(this, Selected_Sound, GetActorLocation(), 1.0f);
		}
		else {
			if (StateEnum == EStateEnum::SE_End) {
				Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->UpDataWidgetText_Skill(this, 1);
				Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->UpDateWidget_SkillPanel(this, 1);
				UGameplayStatics::PlaySoundAtLocation(this, Selected_Sound, GetActorLocation(), 1.0f);
			}
		}
	}
	else {
		UBattleWidget* BW = Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget());
		BW->ChangeSelectedButton(BW->GetSkill1Button());
		Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->UpDataWidgetText_Skill(this, 1);
		Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->UpDateWidget_SkillPanel(this, 1);
		UGameplayStatics::PlaySoundAtLocation(this, Selected_Sound, GetActorLocation(), 1.0f);
	}
}

void APlayerSquadCharacter::SetSkill2()
{
	if ((Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->IsBattleStart != false )) {
		if (Character_Skill2_Cooldown == 0) {
			if (Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->IsBattleStart != false && (StateEnum != EStateEnum::SE_End && StateEnum != EStateEnum::SE_Death)) {
				if (StateEnum == EStateEnum::SE_Shot || StateEnum == EStateEnum::SE_Skill1 || StateEnum == EStateEnum::SE_Skill2)
					Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->SetDisableSkillTargeting(true);
				else if (StateEnum == EStateEnum::SE_Cover || StateEnum == EStateEnum::SE_Reload)
					Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->SetDisableSkillTargeting(false);

				StateEnum = EStateEnum::SE_Skill2;
				SetHighLight(true);
				Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->SetSkillTargeting(true);
			}
				

			Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->UpDataWidgetText_Skill(this, 2);
			Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->UpDateWidget_SkillPanel(this, 2);
			UBattleWidget* BW = Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget());
			BW->ChangeSelectedButton(BW->GetSkill2Button());
			UGameplayStatics::PlaySoundAtLocation(this, Selected_Sound, GetActorLocation(), 1.0f);
		}
		else {
			if (StateEnum == EStateEnum::SE_End) {
				Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->UpDataWidgetText_Skill(this, 2);
				Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->UpDateWidget_SkillPanel(this, 2);
				UGameplayStatics::PlaySoundAtLocation(this, Selected_Sound, GetActorLocation(), 1.0f);
			}
		}
	}
	else {
		UBattleWidget* BW = Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget());
		BW->ChangeSelectedButton(BW->GetSkill2Button());
		Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->UpDataWidgetText_Skill(this, 2);
		Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->UpDateWidget_SkillPanel(this, 2);
		UGameplayStatics::PlaySoundAtLocation(this, Selected_Sound, GetActorLocation(), 1.0f);
	}
}

////////////////////////////////////////////////////////////////////////////

void APlayerSquadCharacter::SetCharacterEnd()
{
	if(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->IsBattleStart == true) {
		auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	
		Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->Set_BattleWidgetOpacity(1.f);
		Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->Set_BattleWidgetSkilliconOpacity(true);
		Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->Set_BattleWidgetSkillButtonActive(true);
		Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->ClearWidget_SkillPanel();
		GetStatustBarWidget()->SetBarRenderOpacity(0.5f);
		StateEnum = EStateEnum::SE_End;
		//Cast<ABattleController>((Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns))->AddPlayerEndBattleArray(this);
		
		UE_LOG(LogClass, Log, TEXT(" Add EndArray " ));
		gameIns->SelectedCharacter = nullptr; // 버그 픽스 추가 04/30
	

		auto SplayerController = Cast<ASquadController>(GetWorld()->GetFirstPlayerController());
		SplayerController->SetSquadControllerInput(true);
		EnableInput(SplayerController);

		gameIns->BCIns->EndTurnSystem();
		//Cast<ABattleController>((Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns))->AddPlayerEndBattleArray(this);
	}
	
}

float APlayerSquadCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage - CharacterDefenceArmor, DamageEvent, EventInstigator, DamageCauser);

	if (LifePoint > 0)
	{
		if(ActualDamage > 0) {
			if (IsActiveBuffCover == true)
			{
				Buff_Cover(BuffOff);
			}
			UCharacterAnimInstance* CharAnimInst = Cast<UCharacterAnimInstance>(animInstance);
			
			if (Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->IsBattleStart == true) {
				CharAnimInst->Hit();
			}
			else {
				if (ActualDamage >= 5) {
					CharAnimInst->Play_StandHit_1();
				}
				else if (CharAnimInst != nullptr)
				{												
						int32 tempRand = FMath::FloorToInt(FMath::RandRange(0.f, 1.9f));
						if (tempRand == 0)
							CharAnimInst->Play_StandHit_2();
						else if (tempRand == 1)
							CharAnimInst->Play_StandHit_3();					
				}
			}
		}
		else if (ActualDamage <= 0) {
			UCharacterAnimInstance* CharAnimInst = Cast<UCharacterAnimInstance>(animInstance);
			CharAnimInst->Hit();
		}
	}
	else
	{
		UCharacterAnimInstance* CharAnimInst = Cast<UCharacterAnimInstance>(animInstance);
		if (CharAnimInst != nullptr)
		{
			PlayerDeath(CharAnimInst);
		}

	}

	return ActualDamage ;
}

void APlayerSquadCharacter::PlayerDeath(UCharacterAnimInstance* CharAnimInst)
{
	//UCharacterAnimInstance* CharAnimInst = Cast<UCharacterAnimInstance>(animInstance);
	USquadGameInstance* gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	Cast<ABattleController>(gameIns->BCIns)->RemoveFromPlayerEndBattleArray(ArrayNumbering, numbering);
	Characterdeath(); // 충돌 무시, 무브먼트 정지 , 상태 변환

	//UGameplayStatics::PlaySoundAtLocation(this, Death_Sound, GetActorLocation(), 1.0f);
	//FString WeaponName = CharacterStat->GetCharacterWeaponName(); [BUG] 11/13 문제점

	if (Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->IsBattleStart == true){
		int32 tempRand = (FMath::RandRange(0.f, 1.9f));
		if (tempRand == 0)
			CharAnimInst->CrouchDeath_1();
		else if (tempRand == 1)
			CharAnimInst->CrouchDeath_2();
	}
	else {
		int32 tempRand = FMath::FloorToInt(FMath::RandRange(0.f, 5.9f));
		if (tempRand == 0)
			CharAnimInst->StandDeath_1();
		else if (tempRand == 1)
			CharAnimInst->StandDeath_2();
		else if (tempRand == 2)
			CharAnimInst->StandDeath_3();
		else if (tempRand == 3)
			CharAnimInst->StandDeath_4();
		else if (tempRand == 4)
			CharAnimInst->StandDeath_5();
		else if (tempRand == 5)
			CharAnimInst->StandDeath_6();
	}

	SetHighLight(false);
	SetHighLight_SelfSkill(false);
	LifeBar->SetHiddenInGame(true);
}

void APlayerSquadCharacter::PlaySelectedSound()
{
	// 
	UGameplayStatics::PlaySoundAtLocation(this, Selected_Sound, GetActorLocation(), 1.0f);
}

void APlayerSquadCharacter::StopMontage()
{
	UCharacterAnimInstance* CharAnimInst = Cast<UCharacterAnimInstance>(animInstance);
	CharAnimInst->StopMontage();
}

void APlayerSquadCharacter::InputTest()
{
	UE_LOG(LogClass, Log, L"!!!");
}

void APlayerSquadCharacter::SetUnderGrid(AGrid* Grid)
{
	UnderGrid = Grid;
	
}

AGrid* APlayerSquadCharacter::GetUnderGrid()
{
	return UnderGrid;
}

int32 APlayerSquadCharacter::GetBattleLineNumber()
{
	return BattleLineNumber;
}

/////////////////////// BUFF ////////////////////////////////

void APlayerSquadCharacter::Buff_System() // 임시로 종료시키는 시스템으로 변경
{
	Buff_Cover(BuffOff);
}

void APlayerSquadCharacter::Buff_Cover(bool Onoff)
{
	// 이미 켜져있을때는?
	 // 종료하게 만든다 - 카운트가 생기면?
	 // 카운트가 0가 아닐시 아무일도 안일어남 - 추후 추가예정
	// 안켜져있을때
	 // 켜지게 하면 됨

	if (Onoff == BuffOff) // 버프 체크용 - 임시로 버프활성화시 버프를 끄는 함수로 변경
	{
		if (IsActiveBuffCover == true) // 버프가 켜져있으면 종료
		{
			CharacterDefenceArmor -= 1.f;
			IsActiveBuffCover = false;
			//UE_LOG(LogClass, Log, L"Buff off");
		}
		else
		{
			
		}
			//UE_LOG(LogClass, Log, L"Buff Not Active");
		// 꺼저있을때는 아무일도 안일어남
		
	}
	else // Onoff == BuffOn 버프 활성화
	{
		if(IsActiveBuffCover == false) // 버프가 비활성화시 버프가 활성화
		{ 
			CharacterDefenceArmor += 1.f;
			IsActiveBuffCover = true;
			//UE_LOG(LogClass, Log, L"Buff On");
		}
		else // 버프가 이미 활성화됬을때는 아무일도 안일어남
		{
			//UE_LOG(LogClass, Log, L"Buff Already Active");
		}
	}
}

//////////////////////////////////////////////////////////////

void APlayerSquadCharacter::BeShowMouseCursor()
{	
	//GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	GetWorld()->GetFirstPlayerController()->EnableInput(GetWorld()->GetFirstPlayerController());
}

void APlayerSquadCharacter::BeHideMouseCursor()
{
	//GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	GetWorld()->GetFirstPlayerController()->DisableInput(Cast<APlayerController>(GetWorld()->GetFirstPlayerController()));
	//GetWorld()->GetFirstPlayerController()->
}

void APlayerSquadCharacter::Calc_Damage_distribution(ASquadCharacter* TargetEvasionCorrection)
{
	// nCr =  n! / (r! * (n-r)!)
	// 0번 성공 = 10! / ( 0! * ( 10 - 0 ) ! ) * (맞을확률)^0 * (빗나갈확률)^10
	Damage_distribution.Init(0.f, 11);
	Damage_distribution_float.Init(0.f, 11);

	for (int i = 0; i <= CharacterStat->GetWeaponFireCount(); i++)
	{
		float FinalCal;
		float maxFireCount = CharacterStat->GetWeaponFireCount();
		float binomial_coefficient = factorial(maxFireCount) / (factorial(i) * factorial(maxFireCount - i));
		float hitACC = (CharacterStat->GetCharacterAccuracyCorrectionValue() + CharacterStat->GetWeaponAccuracy() - TargetEvasionCorrection->Evasion) / 100;
		if (hitACC > 100.f) hitACC = 100.f;
		float nonhitACC = 1 - hitACC;

		FinalCal = binomial_coefficient * FMath::Pow(hitACC, i) * FMath::Pow(nonhitACC, maxFireCount - i);

		
		auto damageNum = i * CharacterStat->GetWeaponDamage();
		Damage_distribution_float[damageNum] = FinalCal;
		FinalCal = FMath::RoundToFloat(FinalCal * 100);
		if (FinalCal > 100) FinalCal = 100;
		else if (FinalCal < 0) FinalCal = 0;
		Damage_distribution[damageNum] = FinalCal;
		
		if (MaxDamage_InDamageDis < FinalCal)
			MaxDamage_InDamageDis = FinalCal;

	
	}
	


	// 구해진건 몇번 맞출 확률
	// 구해야하는건 데미지
}

void APlayerSquadCharacter::Calc_SkillDamage_distribution(ASquadCharacter* Target, struct FSkillValueList* CompSkillData)
{

	SkillDamage_distribution.Init(0.f, 11);
	SkillDamage_distribution_float.Init(0.f, 11);

	// 최대 발사 수 , 정확도 , 
	int32 Skill_ShotCount = CompSkillData->SkillFireCount;
	int32 Skill_BulletDamage = CompSkillData->SkillDamage;
	int32 Skill_BulletCount = CompSkillData->SkillBulletCount;
	int32 Skill_AccuracyCorrections = CompSkillData->Special_ability_1_UsedCharacterAccurancyrateCorrectionValue;	
	int32 Skill_DefenseValue = Target->Defense;
	int32 Skill_CritValue = CompSkillData->Special_ability_2_UsedCharacterCriticalCorrectionValue;

	for (int i = 0; i <= Skill_BulletCount; i++)
	{
		float FinalCal;
		float maxFireCount = Skill_BulletCount;
		float binomial_coefficient = factorial(maxFireCount) / (factorial(i) * factorial(maxFireCount - i));
		float SumAcc = (CharacterStat->GetWeaponAccuracy() + CharacterStat->GetCharacterAccuracyCorrectionValue() + (float)Skill_AccuracyCorrections - Target->Evasion);
			if (SumAcc > 100) SumAcc = 100.f;
			else if (SumAcc < 0) SumAcc = 0.f;
		float hitACC = SumAcc / 100;
		if (hitACC > 100.f) hitACC = 100.f;
		float nonhitACC = 1 - hitACC;

		FinalCal = binomial_coefficient * FMath::Pow(hitACC, i) * FMath::Pow(nonhitACC, maxFireCount - i);


		auto damageNum = i * Skill_BulletDamage;
		SkillDamage_distribution_float[damageNum] = FinalCal;
		FinalCal = FMath::RoundToFloat(FinalCal * 100);
		if (FinalCal > 100) FinalCal = 100;
		else if (FinalCal < 0) FinalCal = 0;
		SkillDamage_distribution[damageNum] = FinalCal;

		if (SkillMaxDamage_InDamageDis < FinalCal)
			SkillMaxDamage_InDamageDis = FinalCal;
	}
}

int APlayerSquadCharacter::factorial(int n)
{
	if (n <= 1)
		return 1;
	else
		return n * factorial(n - 1);
}

////////////

void APlayerSquadCharacter::SetHighLight(bool OnOff)
{
	GetMesh()->SetRenderCustomDepth(OnOff);
	Cap->SetRenderCustomDepth(OnOff);
	Cap_equip->SetRenderCustomDepth(OnOff);
	Head_equip1->SetRenderCustomDepth(OnOff);
	Head_equip2->SetRenderCustomDepth(OnOff);
	Shirt->SetRenderCustomDepth(OnOff);
	Pants->SetRenderCustomDepth(OnOff);
	Boots->SetRenderCustomDepth(OnOff);
	Hands->SetRenderCustomDepth(OnOff);
	Vest->SetRenderCustomDepth(OnOff);
	Vest_outfit->SetRenderCustomDepth(OnOff);
	Backpack->SetRenderCustomDepth(OnOff);
	Backpack2->SetRenderCustomDepth(OnOff);
	Vest_Collar->SetRenderCustomDepth(OnOff);;
	Vest_shoulder_R->SetRenderCustomDepth(OnOff);
	Vest_shoulder_L->SetRenderCustomDepth(OnOff);
	Vest_Bottom->SetRenderCustomDepth(OnOff);
	Decals->SetRenderCustomDepth(OnOff);
	Radio->SetRenderCustomDepth(OnOff);
	Kneepad_R->SetRenderCustomDepth(OnOff);
	Kneepad_L->SetRenderCustomDepth(OnOff);
	Holster->SetRenderCustomDepth(OnOff);
	WeaponSlot->SetRenderCustomDepth(OnOff);

	GetMesh()->SetCustomDepthStencilValue(1);
	Cap->SetCustomDepthStencilValue(1);
	Cap_equip->SetCustomDepthStencilValue(1);
	Head_equip1->SetCustomDepthStencilValue(1);
	Head_equip2->SetCustomDepthStencilValue(1);
	Shirt->SetCustomDepthStencilValue(1);
	Pants->SetCustomDepthStencilValue(1);
	Boots->SetCustomDepthStencilValue(1);
	Hands->SetCustomDepthStencilValue(1);
	Vest->SetCustomDepthStencilValue(1);
	Vest_outfit->SetCustomDepthStencilValue(1);
	Backpack->SetCustomDepthStencilValue(1);
	Backpack2->SetCustomDepthStencilValue(1);
	Vest_Collar->SetCustomDepthStencilValue(1);;
	Vest_shoulder_R->SetCustomDepthStencilValue(1);
	Vest_shoulder_L->SetCustomDepthStencilValue(1);
	Vest_Bottom->SetCustomDepthStencilValue(1);
	Decals->SetCustomDepthStencilValue(1);
	Radio->SetCustomDepthStencilValue(1);
	Kneepad_R->SetCustomDepthStencilValue(1);
	Kneepad_L->SetCustomDepthStencilValue(1);
	Holster->SetCustomDepthStencilValue(1);
	WeaponSlot->SetCustomDepthStencilValue(1);

}

void APlayerSquadCharacter::SetHighLight_SelfSkill(bool OnOff)
{
	GetMesh()->SetRenderCustomDepth(OnOff);
	Cap->SetRenderCustomDepth(OnOff);
	Cap_equip->SetRenderCustomDepth(OnOff);
	Head_equip1->SetRenderCustomDepth(OnOff);
	Head_equip2->SetRenderCustomDepth(OnOff);
	Shirt->SetRenderCustomDepth(OnOff);
	Pants->SetRenderCustomDepth(OnOff);
	Boots->SetRenderCustomDepth(OnOff);
	Hands->SetRenderCustomDepth(OnOff);
	Vest->SetRenderCustomDepth(OnOff);
	Vest_outfit->SetRenderCustomDepth(OnOff);
	Backpack->SetRenderCustomDepth(OnOff);
	Backpack2->SetRenderCustomDepth(OnOff);
	Vest_Collar->SetRenderCustomDepth(OnOff);;
	Vest_shoulder_R->SetRenderCustomDepth(OnOff);
	Vest_shoulder_L->SetRenderCustomDepth(OnOff);
	Vest_Bottom->SetRenderCustomDepth(OnOff);
	Decals->SetRenderCustomDepth(OnOff);
	Radio->SetRenderCustomDepth(OnOff);
	Kneepad_R->SetRenderCustomDepth(OnOff);
	Kneepad_L->SetRenderCustomDepth(OnOff);
	Holster->SetRenderCustomDepth(OnOff);
	WeaponSlot->SetRenderCustomDepth(OnOff);

	GetMesh()->SetCustomDepthStencilValue(3);
	Cap->SetCustomDepthStencilValue(3);
	Cap_equip->SetCustomDepthStencilValue(3);
	Head_equip1->SetCustomDepthStencilValue(3);
	Head_equip2->SetCustomDepthStencilValue(3);
	Shirt->SetCustomDepthStencilValue(3);
	Pants->SetCustomDepthStencilValue(3);
	Boots->SetCustomDepthStencilValue(3);
	Hands->SetCustomDepthStencilValue(3);
	Vest->SetCustomDepthStencilValue(3);
	Vest_outfit->SetCustomDepthStencilValue(3);
	Backpack->SetCustomDepthStencilValue(3);
	Backpack2->SetCustomDepthStencilValue(3);
	Vest_Collar->SetCustomDepthStencilValue(3);;
	Vest_shoulder_R->SetCustomDepthStencilValue(3);
	Vest_shoulder_L->SetCustomDepthStencilValue(3);
	Vest_Bottom->SetCustomDepthStencilValue(3);
	Decals->SetCustomDepthStencilValue(3);
	Radio->SetCustomDepthStencilValue(3);
	Kneepad_R->SetCustomDepthStencilValue(3);
	Kneepad_L->SetCustomDepthStencilValue(3);
	Holster->SetCustomDepthStencilValue(3);
	WeaponSlot->SetCustomDepthStencilValue(3);

}

void APlayerSquadCharacter::SetTurnOnHighLightGrid()
{

}

void APlayerSquadCharacter::SetTurnOffHighLightGrid()
{
	if (UnderGrid != nullptr)
		UnderGrid->SetGridInfo_Material_Black();
}

//////////// 스킬 /////////////////

void APlayerSquadCharacter::SetPlayerSkill_ClassNum(int32 ClassNum)
{
	CharacterSkillComp->SetCharacterData(ClassNum, this);
	CharacterSkillComp->InitCharacterSkill();
}

void APlayerSquadCharacter::SetSkillNumAndTarget(int32 skillNum, AActor* TargetCharacter)
{
	this->skillNum = skillNum;
	this->SkillTargetCharacter = TargetCharacter;
	tempTargetCharacter = Cast<ASquadCharacter>(TargetCharacter);
	UCharacterAnimInstance* CharAnim = Cast<UCharacterAnimInstance>(animInstance);

	Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->Set_BattleWidgetOpacity(0.5f);
	Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->Set_BattleWidgetSkilliconOpacity(false);
	Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->Set_BattleWidgetSkillButtonActive(false);

	IsCharacterUseAttack = true;
	Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->Set_BattleWidgetSkillButtonActive(false);
	CharAnim->Play_SkillReadyMontage();

}

void APlayerSquadCharacter::UsePlayerSkill(int32 skillNum, AActor* TargetCharacter)
{
	UCharacterAnimInstance* CharAnim = Cast<UCharacterAnimInstance>(animInstance);
	FString WeaponName = CharacterStat->GetCharacterWeaponName();
	
	

	if (WeaponName == "Rifle") {
		
		CharAnim->Play_SkillFiringMontage();
	}
	else if (WeaponName == "Pistol") {
		
		CharAnim->Play_SkillFiringMontage();
	}
	else if (WeaponName == "Shotgun") {
		
		CharAnim->Play_SkillFireMontage();
	}
	else if (WeaponName == "Sniper") {
		
		CharAnim->Play_SkillFireMontage();
	}
	//CharAnim->Play_SkillFiringMontage();

	CharacterSkillComp->UseCharacterSkill(this->skillNum, this->SkillTargetCharacter);

	
	
	this->skillNum = NULL;
	this->SkillTargetCharacter = nullptr;
	//SetCharacterEnd();// 임시
}




/////////////////////////////////////

void APlayerSquadCharacter::DebugMessage_CharacterState()
{
}

void APlayerSquadCharacter::SetIsCharacterUseAttackTotrue()
{
	IsCharacterUseAttack = false;
	tempTargetCharacter = nullptr;
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySquadCharacter.h"
#include "CharacterAnimInstance.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Engine/SkeletalMesh.h"
#include "SquadGameInstance.h"
#include "Grid.h"
#include "BattleController.h"
#include "SquadGameMode.h"
#include "Engine/Engine.h"
#include "PlayerSquadCharacter.h"
#include "SquadCharacterStatComponent.h"


AEnemySquadCharacter::AEnemySquadCharacter()
{
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
	
	FName WeaponSocket(TEXT("hand_rSocket"));
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> Assault_Rifle(L"SkeletalMesh'/Game/MilitaryWeapDark/Weapons/Assault_Rifle_B.Assault_Rifle_B'");
		if (Assault_Rifle.Succeeded())
		{
			Weapon->SetSkeletalMesh(Assault_Rifle.Object);
		}
		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> RECRUIT_ANIM(
		TEXT("AnimBlueprint'/Game/ANIMATION/EnemyBP/EC_Recruit.EC_Recruit_C'"));
	if (RECRUIT_ANIM.Succeeded())
	{
		RecruitAnimBP = RECRUIT_ANIM.Class;
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> RIFLE_ANIM(
		TEXT("AnimBlueprint'/Game/ANIMATION/EnemyBP/EC_RifleMan.EC_RifleMan_C'"));
	if (RIFLE_ANIM.Succeeded())
	{
		RifleAnimBP = RIFLE_ANIM.Class;
	}

	
	static ConstructorHelpers::FClassFinder<UAnimInstance> PISTOL_ANIM(
		TEXT("AnimBlueprint'/Game/ANIMATION/EnemyBP/EC_Police.EC_Police_C'"));
	if (PISTOL_ANIM.Succeeded())
	{
		PistolAnimBP = PISTOL_ANIM.Class;
	}
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> SHOTGUN_ANIM(
		TEXT("AnimBlueprint'/Game/ANIMATION/EnemyBP/EC_Shotgun.EC_Shotgun_C'"));
	if (SHOTGUN_ANIM.Succeeded())
	{
		ShotgunAnimBP = SHOTGUN_ANIM.Class;
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> SNIPER_ANIM(
		TEXT("AnimBlueprint'/Game/ANIMATION/EnemyBP/EC_Sniper.EC_Sniper_C'"));
	if (SNIPER_ANIM.Succeeded())
	{
		SniperAnimBP = SNIPER_ANIM.Class;
	}
	
	//// Init Enemy Character Stat ////
	LifePoint = 0.f;
	Accuracy = 0.f;
	Evasion = 0.f;
	Critical = 0.f;
	Damage = 0.f;
	CurrentAmmo = 0.f;
	FireCount = 0.f;
}

void AEnemySquadCharacter::BeginPlay()
{
	Super::BeginPlay();
	Character_Rotator_StartRotator = GetActorRotation();
}

void AEnemySquadCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsCharacterUseAttack == false) {
		SetActorRotation(FMath::Lerp(GetActorRotation(), Character_Rotator_StartRotator, DeltaTime * 10));
	}
	if (IsCharacterUseAttack == true) {
		FVector CharLoc = GetActorLocation();
		FVector TargetLoc = tempTargetCharacter->GetActorLocation();
		FVector Rot = TargetLoc - CharLoc;
		auto InRot = FMath::RInterpTo(GetActorRotation(), Rot.ToOrientationRotator(), DeltaTime, 10.f);
		SetActorRotation(InRot);
	}
}

float AEnemySquadCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (LifePoint > 0) {
		if(ActualDamage > 0) {
			UCharacterAnimInstance* CharAnimInst = Cast<UCharacterAnimInstance>(animInstance);
			if (CharAnimInst != nullptr) {
				CharAnimInst->Enemy_Hit();
			}
		}
		else if (ActualDamage <= 0) {
			UCharacterAnimInstance* CharAnimInst = Cast<UCharacterAnimInstance>(animInstance);
			if (CharAnimInst != nullptr) {
				CharAnimInst->Enemy_Hit();
			}
		}
	}
	else {
		UCharacterAnimInstance* CharAnimInst = Cast<UCharacterAnimInstance>(animInstance);
		if (CharAnimInst != nullptr) {
			EnemyDeath(CharAnimInst);
		}
	}

	return ActualDamage;
}

void AEnemySquadCharacter::EnemyDeath(UCharacterAnimInstance* CharAnimInst)
{
	if(!IsDeath) {
		USquadGameInstance* gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
		// 배열 숫자 변경
		ArrayNumbering = 999; 
		// BattleControoler의 적 종료 배열에서 삭제
		Cast<ABattleController>(gameIns->BCIns)->RemoveFromEnemyEndBattleArray(this);
		// 밟고 있던 그리드 visiblity 변경
		SetGridOff();
		// 캐릭터 사망처리
		Characterdeath(); // 충돌 무시, 무브먼트 정지 , 상태 변환

		// 랜덤하게 캐릭터 사망 모션 연출
		int32 tempRand = FMath::FloorToInt(FMath::RandRange(0.f, 1.9f));
		if (tempRand == 0)
			CharAnimInst->Enemy_Death1();
		else if (tempRand == 1)
			CharAnimInst->Enemy_Death2();
		// 캐릭터 HP바 숨기기
		LifeBar->SetHiddenInGame(true);
		// 인스턴스의 적 캐릭터 사망 카운트
		gameIns->BCIns->AddEnemyDeathCount();
	}
}

void AEnemySquadCharacter::SetBelongToBattleTrigger(ABattleTrigger* BattleTrigger)
{
	BelongToBattleTrigger = BattleTrigger;
}

ABattleTrigger* AEnemySquadCharacter::GetBelongToBattleTrigger()
{
	return BelongToBattleTrigger;
}

void AEnemySquadCharacter::Enemy_ReadytoShot(class APlayerSquadCharacter* shotTarget)
{	
	IsCharacterUseAttack = true;
	UCharacterAnimInstance* CharAnimInst = Cast<UCharacterAnimInstance>(animInstance);
	if (CurrentAmmo <= 0) {
		CharacterSkill_Reload();   // 리로드			
	}
	else {
		tempshotTarget = shotTarget;
		if(CharAnimInst != nullptr)
			CharAnimInst->Enemy_Aiming();
	}

}

void AEnemySquadCharacter::Enemy_Shot(AActor* Target)
{
	UCharacterAnimInstance* CharAnim = Cast<UCharacterAnimInstance>(animInstance);		
	
	float FinalDamage = 0.f;
	float TargetTotalAcc = tempshotTarget->CharacterStat->GetCharacterEvasionCorrectionValue() + tempshotTarget->CharacterStat->GetWeaponAvoidence();

	for (int i = 0; i < FireCount; i++) {
		if (FMath::RandRange(0.f, 100.f) < Accuracy - TargetTotalAcc) {
			FinalDamage += Damage;
		}
	}

	CurrentAmmo -= 1.f;

	if (weaponNum == 0)
		CharAnim->Enemy_BeShoting();
	else
		CharAnim->Enemy_BeShoting();			

	if (FinalDamage > 0)
		UGameplayStatics::ApplyDamage(tempshotTarget, FinalDamage, GetWorld()->GetFirstPlayerController(), this, nullptr);
	else if (FinalDamage == 0) {
		FDamageEvent DamegeEvent;
		(tempshotTarget)->TakeDamage(0, DamegeEvent, nullptr, this);
	}
		
	tempshotTarget = nullptr;		
}

void AEnemySquadCharacter::Enemy_TurnEnd()
{
	IsCharacterUseAttack = false;
	tempTargetCharacter = nullptr;
	Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->ActiveEnemyAI();
}

void AEnemySquadCharacter::CharacterSkill_Reload()
{
	CurrentAmmo = MaxAmmo;
	UCharacterAnimInstance* CharAnim = Cast<UCharacterAnimInstance>(animInstance);
	CharAnim->Enemy_Reloading();

	ShotTarget = nullptr;

	// 장전 관련
	// - 애니메이션 추가 필요
	// - 애니메이션에 노티파이를 붙여서 관리할것인가 ( 애니메이션 재생 시간 + 턴 종료 시간 )
	// - 그냥 애니메이션 재생만 할것인가 ( 턴 종료 시간 )
	// => 애니메이션에 노티파이를 붙여서 관리하기로 정해짐
}

void AEnemySquadCharacter::SetUnderGrid(AGrid* Grid)
{
	UnderGrid = Grid;
}

AGrid* AEnemySquadCharacter::GetUnderGrid()
{
	return UnderGrid;
}

void AEnemySquadCharacter::SetWeaponStat(FString WeaponName)
{
	// 1.병과에 따른 소지하고 있는 총, 총알 연출 변경
	// 2.총의 스테이터스를 캐릭터 능력치에 추가

	auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());	

	// 1. 총,총알, 애니메이션 연출 변경
	if (WeaponName == "Rifle") {
		weaponNum = 0;
		ProjectileBulletClass = AssaultBulletClass;
		SetWeaponMesh(L"SkeletalMesh'/Game/Characters/Military/Mesh/Weapon/SK_Military_Rifle2.SK_Military_Rifle2'");
		
		GetMesh()->SetAnimInstanceClass(RifleAnimBP);
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		animInstance = GetMesh()->GetAnimInstance();
	}
	else if (WeaponName == "Pistol") {
		weaponNum = 1;
		ProjectileBulletClass = AssaultBulletClass;
		SetWeaponMesh(L"SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/KA_Val/SK_KA_Val_Y.SK_KA_Val_Y'");
	
		GetMesh()->SetAnimInstanceClass(PistolAnimBP);
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		animInstance = GetMesh()->GetAnimInstance();
	}
	else if (WeaponName == "Shotgun") {
		weaponNum = 2;
		ProjectileBulletClass = ShotgunBulletClass;
		SetWeaponMesh(L"SkeletalMesh'/Game/MilitaryWeapDark/Weapons/Shotgun_B.Shotgun_B'");

		GetMesh()->SetAnimInstanceClass(ShotgunAnimBP);
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		animInstance = GetMesh()->GetAnimInstance();
	}
	else if (WeaponName == "Sniper") {
		weaponNum = 3;
		ProjectileBulletClass = SniperBulletClass;
		SetWeaponMesh(L"SkeletalMesh'/Game/Characters/CharacterWeapon/Sniper/Sniper_Rifle_B.Sniper_Rifle_B'");
		GetMesh()->SetAnimInstanceClass(SniperAnimBP);
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		animInstance = GetMesh()->GetAnimInstance();
	}

	// 2. DT에서 총의 스테이터스를 읽어봐 능력치에 합산
	auto WeaponDT = gameIns->GetWeaponData(weaponNum);
	
	LifePoint = 10.f;
	Accuracy += WeaponDT->WeaponAccuracy;
	Evasion += WeaponDT->WeaponAvoidence;
	Critical = WeaponDT->WeaponCritical;
	Damage = WeaponDT->WeaponDamage;
	CurrentAmmo = WeaponDT->WeaponMaxFireCount; // ammo
	FireCount = WeaponDT->WeaponFireCount;

	MaxAmmo = CurrentAmmo;
	MaxLifePoint = LifePoint;
}

void AEnemySquadCharacter::SetSkeletalMesh(const TCHAR* ContentPath)
{
	if (ContentPath != nullptr)	{
		USkeletalMesh* CharacterMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), NULL, ContentPath));
		if (CharacterMesh) {
			GetMesh()->SetSkeletalMesh(CharacterMesh);
		}
		else {

		}
	}
}

void AEnemySquadCharacter::SetWeaponMesh(const TCHAR* ContentPath)
{
	if (ContentPath != nullptr)	{
		USkeletalMesh* CharacterMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), NULL, ContentPath));
		if (CharacterMesh)	{
			Weapon->SetSkeletalMesh(CharacterMesh);
		}
		else {

		}
	}
}

void AEnemySquadCharacter::SetSkeletalMeshPath(int32 BrunchNum)
{
	
	switch (BrunchNum)
	{
		case 0:
			SetSkeletalMesh(L"SkeletalMesh'/Game/Characters/CharacterBP/SM_Enemy_Recruit.SM_Enemy_Recruit'");
			break;
		case 1:
			SetSkeletalMesh(L"SkeletalMesh'/Game/Characters/CharacterBP/SM_Enemy_RecruitLeader.SM_Enemy_RecruitLeader'");
			break;
		case 2:
			SetSkeletalMesh(L"SkeletalMesh'/Game/Characters/CharacterBP/SM_Enemy_Rifleman.SM_Enemy_Rifleman'");
			break;
		case 4:
			SetSkeletalMesh(L"SkeletalMesh'/Game/Characters/CharacterBP/SM_Enemy_Police.SM_Enemy_Police'");
			break;
		case 6:
			SetSkeletalMesh(L"SkeletalMesh'/Game/Characters/CharacterBP/SM_Enemy_Assault.SM_Enemy_Assault'");
			break;
		case 8:
			SetSkeletalMesh(L"SkeletalMesh'/Game/Characters/CharacterBP/SM_Enemy_Sniper.SM_Enemy_Sniper'");
			break;
		default:
			break;
	}
}

void AEnemySquadCharacter::SetBrunchAnimBP()
{

}

void AEnemySquadCharacter::SetHighLight(bool OnOff)
{	
	GetMesh()->SetRenderCustomDepth(OnOff);
	Weapon->SetRenderCustomDepth(OnOff);
	GetMesh()->SetCustomDepthStencilValue(2);
	Weapon->SetCustomDepthStencilValue(2);

	SetbIsHighLight(OnOff);
}
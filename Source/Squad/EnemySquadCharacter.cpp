// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySquadCharacter.h"
#include "CharacterAnimInstance.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "SquadGameInstance.h"
#include "Grid.h"
#include "BattleController.h"


AEnemySquadCharacter::AEnemySquadCharacter()
{
	LifePoint = 3;
	/*
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("GUNSOUND"));
	AudioComp->SetupAttachment(RootComponent);
	*/

	static ConstructorHelpers::FObjectFinder<USoundBase> FireSound(L"SoundWave'/Game/GunSoundPack/Guns/gun_rifle_shot_01.gun_rifle_shot_01'");
	if (FireSound.Succeeded())
	{
		Fire_Sound = FireSound.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> GetHitSound(L"SoundWave'/Game/HumanMaleA/Wavs/voice_male_grunt_pain_12.voice_male_grunt_pain_12'");
	if (GetHitSound.Succeeded())
	{
		GetHit_Sound = GetHitSound.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> DeathSound(L"SoundWave'/Game/HumanMaleA/Wavs/voice_male_grunt_pain_death_08.voice_male_grunt_pain_death_08'");
	if (DeathSound.Succeeded())
	{
		Death_Sound = DeathSound.Object;
	}
	/*
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
	*/

}

float AEnemySquadCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (LifePoint > 0)
	{
		UCharacterAnimInstance* CharAnimInst = Cast<UCharacterAnimInstance>(animInstance);
		if (CharAnimInst != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, GetHit_Sound, GetActorLocation(), 1.0f);
			CharAnimInst->Enemy_Hit();
		}
	}
	else
	{
		UCharacterAnimInstance* CharAnimInst = Cast<UCharacterAnimInstance>(animInstance);
		if (CharAnimInst != nullptr)
		{
			EnemyDeath(CharAnimInst);
		}

	}

	return ActualDamage;
}

void AEnemySquadCharacter::EnemyDeath(UCharacterAnimInstance* CharAnimInst)
{
	//UCharacterAnimInstance* CharAnimInst = Cast<UCharacterAnimInstance>(animInstance);
	USquadGameInstance* gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	Cast<ABattleController>(gameIns->BCIns)->RemoveFromEnemyEndBattleArray(ArrayNumbering);
	Characterdeath(); // 충돌 무시, 무브먼트 정지 , 상태 변환

	UGameplayStatics::PlaySoundAtLocation(this, Death_Sound, GetActorLocation(), 1.0f);
	CharAnimInst->Enemy_Death();

	
	LifeBar->SetHiddenInGame(true);
	

	if (Fun_Death.IsBound())
	{
		Fun_Death.Execute();
	}

	Fun_Death.Unbind();

	
}
	
void AEnemySquadCharacter::TestLog()
{
	auto Loc = GetActorLocation().ToString();
	UE_LOG(LogClass, Log, L"%s", *Loc);
}

void AEnemySquadCharacter::SetBelongToBattleTrigger(ABattleTrigger* BattleTrigger)
{
	BelongToBattleTrigger = BattleTrigger;
}

ABattleTrigger* AEnemySquadCharacter::GetBelongToBattleTrigger()
{
	return BelongToBattleTrigger;
}

void AEnemySquadCharacter::Enemy_Shot(AActor* Target)
{
	    UCharacterAnimInstance* CharAnim = Cast<UCharacterAnimInstance>(animInstance);
	
	

		CharAnim->Enemy_BeShot();
		GameStatic->SpawnEmitterAttached(FireParticle, Weapon, FName("MuzzleFlash"));
		UGameplayStatics::PlaySoundAtLocation(this, Fire_Sound, GetActorLocation(), 0.2f);
		UGameplayStatics::ApplyDamage(Target, Damage, GetWorld()->GetFirstPlayerController(), this, nullptr);
		

}

void AEnemySquadCharacter::SetUnderGrid(AGrid* Grid)
{
	UnderGrid = Grid;
}

AGrid* AEnemySquadCharacter::GetUnderGrid()
{
	return UnderGrid;
}
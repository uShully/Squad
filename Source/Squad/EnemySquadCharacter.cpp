// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySquadCharacter.h"
#include "CharacterAnimInstance.h"


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
			CharAnimInst->Hit();
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

	Characterdeath();

	UGameplayStatics::PlaySoundAtLocation(this, Death_Sound, GetActorLocation(), 1.0f);
	CharAnimInst->Death();

	StateEnum = EStateEnum::SE_Death;

	if (Fun_Death.IsBound())
	{
		Fun_Death.Execute();
		UE_LOG(LogClass, Log, L"Death");
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

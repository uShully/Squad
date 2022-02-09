// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerSquadCharacter.h"
#include "CharacterAnimInstance.h"
#include "SquadGameInstance.h"
#include "Grid.h"
//#include "Kismet/GameplayStatics.h"
//#include "UObject/ConstructorHelpers.h"

APlayerSquadCharacter::APlayerSquadCharacter()
{
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		PlayerCharacterAnimBP(TEXT("Blueprint'/Game/DevFile/PlayerCharacterAnimInstance.PlayerCharacterAnimInstance_C'"));
	if (PlayerCharacterAnimBP.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(PlayerCharacterAnimBP.Class);
		UE_LOG(LogClass, Log, TEXT(" Anim Succeeded "));
	}
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

	static ConstructorHelpers::FObjectFinder<USoundBase> SelectedSound(L"SoundWave'/Game/HumanMaleA/Wavs/voice_male_soldier_enemy_spotted_01.voice_male_soldier_enemy_spotted_01'");
	if (SelectedSound.Succeeded())
	{
		Selected_Sound = SelectedSound.Object;
	}
	

	/*
	// particleSystem

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");
	ParticleSystem->SetupAttachment(RootComponent);
	ParticleSystem->bAutoActivate = false;
	// ParticleSystem->SetRelativeLocation(FVector(-20.0f, 0.0f, 20.0f);
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(L"");
	if (ParticleAsset.Succeeded())
	{
		ParticleSystem->SetTemplate(ParticleAsset.Object);
	}
	*/

}

////////////////////////////// Shot //////////////////////////////////////////////

void APlayerSquadCharacter::SetShotReady()
{
	StateEnum = EStateEnum::SE_Shot;

	UCharacterAnimInstance* CharAnimInst = Cast<UCharacterAnimInstance>(animInstance);
	if (CharAnimInst != nullptr)
	{
		CharAnimInst->Aiming();
	}
}

void APlayerSquadCharacter::Debug_Shot(ASquadCharacter* Target)
{
	auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	
	if(gameIns->TargetCharacter != nullptr)
	{
		UCharacterAnimInstance* CharAnim = Cast<UCharacterAnimInstance>(animInstance);
	
		CharAnim->BeShot();
		GameStatic->SpawnEmitterAttached(FireParticle, Weapon, FName("MuzzleFlash"));

		UGameplayStatics::PlaySoundAtLocation(this, Fire_Sound, GetActorLocation(), 0.2f);
	
		UGameplayStatics::ApplyDamage(Target, 1.0f, GetWorld()->GetFirstPlayerController(), this, nullptr);
	}

	if(pGridOnCharacter != nullptr)
	pGridOnCharacter->SetGridInfo_Material_temp2();
}

////////////////////////////// Move //////////////////////////////////////////////

void APlayerSquadCharacter::SetMoveReady()
{
	StateEnum = EStateEnum::SE_Move;


}



void APlayerSquadCharacter::SetStay()
{
	StateEnum = EStateEnum::SE_Stay;

	
	UCharacterAnimInstance* CharAnim = Cast<UCharacterAnimInstance>(animInstance);
	CharAnim->Montage_Stop(0.5f);

}

void APlayerSquadCharacter::SetCharacterEnd()
{
	StateEnum = EStateEnum::SE_End;
}

float APlayerSquadCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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
			UGameplayStatics::PlaySoundAtLocation(this, Death_Sound, GetActorLocation(), 1.0f);
			CharAnimInst->Death();
		}

	}

	return ActualDamage;
}

void APlayerSquadCharacter::PlaySelectedSound()
{
	// 
	UGameplayStatics::PlaySoundAtLocation(this, Selected_Sound, GetActorLocation(), 1.0f);
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
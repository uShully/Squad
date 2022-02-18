// Fill out your copyright notice in the Description page of Project Settings.

#define BuffOn  true
#define BuffOff false

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
		UGameplayStatics::ApplyDamage(Target, Damage, GetWorld()->GetFirstPlayerController(), this, nullptr);
	}

	SetCharacterEnd();

	if(UnderGrid != nullptr)
		UnderGrid->SetGridInfo_Material_temp2();
}

////////////////////////////// Move //////////////////////////////////////////////

void APlayerSquadCharacter::SetMoveReady()
{
	StateEnum = EStateEnum::SE_Move;


}

////////////////////////////// Cover //////////////////////////////////////////////

void APlayerSquadCharacter::SetCover()
{
	if(StateEnum == EStateEnum::SE_Stay)
	{
		UCharacterAnimInstance* CharAnim = Cast<UCharacterAnimInstance>(animInstance);
		auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

		CharAnim->BeCover();
		Buff_Cover(BuffOn);
		SetCharacterEnd();

		if (UnderGrid != nullptr)
			UnderGrid->SetGridInfo_Material_temp2();

		gameIns->BCIns->EndTurnSystem();
	}
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
	Cast<ABattleController>((Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns))->AddPlayerEndBattleArray(this);
	UE_LOG(LogClass, Log, TEXT(" Add EndArray " ));
}

float APlayerSquadCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage - CharacterDefenceArmor, DamageEvent, EventInstigator, DamageCauser);

	if (LifePoint > 0)
	{
		if (IsActiveBuffCover == true)
		{
			Buff_Cover(BuffOff);
		}
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

	UGameplayStatics::PlaySoundAtLocation(this, Death_Sound, GetActorLocation(), 1.0f);
	CharAnimInst->Death();

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
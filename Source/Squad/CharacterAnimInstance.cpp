// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterAnimInstance.h"
#include "UObject/ConstructorHelpers.h"

UCharacterAnimInstance::UCharacterAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Shot_Montage(L"AnimMontage'/Game/Animation/PlayerCharacterAnim/PlayerCharacter_Combat_Shoot_Montage.PlayerCharacter_Combat_Shoot_Montage'");
	if (Shot_Montage.Succeeded())
	{
		shotMontage = Shot_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> GetHit_Montage(L"AnimMontage'/Game/Animation/PlayerCharacterAnim/Hit_React_1_Montage.Hit_React_1_Montage'");
	if (GetHit_Montage.Succeeded())
	{
		gethitMontage = GetHit_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Death_Montage(L"AnimMontage'/Game/Animation/PlayerCharacterAnim/Combat_Dying_Montage.Combat_Dying_Montage'");
	if(Death_Montage.Succeeded())
	{
		deathMontage = Death_Montage.Object;
	}	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Aim_Montage(L"AnimMontage'/Game/Animation/PlayerCharacterAnim/PlayerCharacter_Aiming_Idle_Montage.PlayerCharacter_Aiming_Idle_Montage'");
	if (Aim_Montage.Succeeded())
	{
		aimingMontage = Aim_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Cover_Montage(L"AnimMontage'/Game/Animation/PlayerCharacterAnim/Stand_to_Crouch_Rifle_Ironsights_Montage.Stand_to_Crouch_Rifle_Ironsights_Montage'");
	if (Cover_Montage.Succeeded())
	{
		coverMontage = Cover_Montage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Enemy_Shot_Montage(L"AnimMontage'/Game/Animation/EnemyCharacterAnim/Combat_Shoot_Montage.Combat_Shoot_Montage'");
	if (Enemy_Shot_Montage.Succeeded())
	{
		Enemy_shotMontage = Enemy_Shot_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Enemy_GetHit_Montage(L"AnimMontage'/Game/Animation/EnemyCharacterAnim/Hit_React_1_Montage.Hit_React_1_Montage'");
	if (Enemy_GetHit_Montage.Succeeded())
	{
		Enemy_HitMontage = Enemy_GetHit_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Enemy_Death_Montage(L"AnimMontage'/Game/Animation/EnemyCharacterAnim/Combat_Dying_Montage.Combat_Dying_Montage'");
	if (Enemy_Death_Montage.Succeeded())
	{
		Enemy_DeathMontage = Enemy_Death_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Enemy_Aim_Montage(L"AnimMontage'/Game/Animation/EnemyCharacterAnim/Rifle_Aiming_Idle_Montage.Rifle_Aiming_Idle_Montage'");
	if (Enemy_Aim_Montage.Succeeded())
	{
		Enemy_AimingMontage = Enemy_Aim_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Enemy_Cover_Montage(L"AnimMontage'/Game/Animation/EnemyCharacterAnim/Stand_to_Crouch_Rifle_Ironsights_Montage.Stand_to_Crouch_Rifle_Ironsights_Montage'");
	if (Enemy_Cover_Montage.Succeeded())
	{
		Enemy_BeCoverMontage = Enemy_Cover_Montage.Object;
	}


}

void UCharacterAnimInstance::BeShot()
{
	if(shotMontage != nullptr)
	Montage_Play(shotMontage, 0.7f);
}

void UCharacterAnimInstance::BeCover()
{
	Montage_Play(coverMontage, 1.f);
}

void UCharacterAnimInstance::Hit()
{
	Montage_Play(gethitMontage, 2.0f);

	
}

void UCharacterAnimInstance::Death()
{
	Montage_Play(deathMontage, 1.0);

}

void UCharacterAnimInstance::Aiming()
{
	Montage_Play(aimingMontage, 1.0f);
}

void UCharacterAnimInstance::Enemy_BeShot()
{
	Montage_Play(Enemy_shotMontage, 0.7f);
}

void UCharacterAnimInstance::Enemy_BeCover()
{
	Montage_Play(Enemy_BeCoverMontage, 1.0f);
}

void UCharacterAnimInstance::Enemy_Hit()
{
	Montage_Play(Enemy_HitMontage, 2.0f);
}

void UCharacterAnimInstance::Enemy_Death()
{
	Montage_Play(Enemy_DeathMontage, 1.f);
}

void UCharacterAnimInstance::Enemy_Aiming()
{
	Montage_Play(Enemy_AimingMontage, 1.0f);
}

void UCharacterAnimInstance::StopMontage()
{
	StopAllMontages(0.5f);
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterAnimInstance.h"
#include "UObject/ConstructorHelpers.h"

UCharacterAnimInstance::UCharacterAnimInstance()
{


	static ConstructorHelpers::FObjectFinder<UAnimMontage> Shot_Montage(L"AnimMontage'/Game/DevFile/Animation/Combat_Shoot_Montage.Combat_Shoot_Montage'");
	if (Shot_Montage.Succeeded())
	{
		shotMontage = Shot_Montage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> GetHit_Montage(L"AnimMontage'/Game/DevFile/Animation/Hit_Reaction_Montage.Hit_Reaction_Montage'");
	if (GetHit_Montage.Succeeded())
	{
		gethitMontage = GetHit_Montage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Death_Montage(L"AnimMontage'/Game/DevFile/Animation/Combat_Dying_Montage.Combat_Dying_Montage'");
	if(Death_Montage.Succeeded())
	{
		deathMontage = Death_Montage.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Aim_Montage(L"AnimMontage'/Game/DevFile/Animation/Rifle_Aiming_Idle_Montage.Rifle_Aiming_Idle_Montage'");
	if (Aim_Montage.Succeeded())
	{
		aimingMontage = Aim_Montage.Object;
	}
}

void UCharacterAnimInstance::BeShot()
{
	Montage_Play(shotMontage, 0.7f);
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
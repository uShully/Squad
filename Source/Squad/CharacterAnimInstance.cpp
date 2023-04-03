// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterAnimInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "PlayerSquadCharacter.h"
#include "SquadCharacterStatComponent.h"
#include "Engine/Engine.h"

UCharacterAnimInstance::UCharacterAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ShotRifle_Montage(L"AnimMontage'/Game/ANIMATION/AnimList/AnimExplore/StandFiring_Common_Montage_Rifle.StandFiring_Common_Montage_Rifle'");
	if (ShotRifle_Montage.Succeeded())
	{
		shotRifleMontage = ShotRifle_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ShotPistol_Montage(L"AnimMontage'/Game/ANIMATION/AnimList/AnimExplore/StandFiring_Common_Montage_Pistol.StandFiring_Common_Montage_Pistol'");
	if (ShotPistol_Montage.Succeeded())
	{
		shotPistolMontage = ShotPistol_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ShotShotgun_Montage(L"AnimMontage'/Game/ANIMATION/AnimList/AnimExplore/StandFire_Common_Montage_Shotgun.StandFire_Common_Montage_Shotgun'");
	if (ShotShotgun_Montage.Succeeded())
	{
		shotShotgunMontage = ShotShotgun_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ShotSniper_Montage(L"AnimMontage'/Game/ANIMATION/AnimList/AnimExplore/StandFire_Common_Montage_Sniper.StandFire_Common_Montage_Sniper'");
	if (ShotSniper_Montage.Succeeded())
	{
		shotSniperMontage = ShotSniper_Montage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> GetHit_Montage(L"AnimMontage'/Game/ANIMATION/AnimList/AnimBattle/CrouchHit_Common_Montage.CrouchHit_Common_Montage'");
	if (GetHit_Montage.Succeeded())
	{
		gethitMontage = GetHit_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> GetHit2_Montage(L"AnimMontage'/Game/ANIMATION/PlayerCharacterAnim/Hit_React_2_Montage.Hit_React_2_Montage'");
	if (GetHit2_Montage.Succeeded())
	{
		gethitMontage_2 = GetHit2_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> GetHit3_Montage(L"AnimMontage'/Game/ANIMATION/PlayerCharacterAnim/Hit_React_3_Montage.Hit_React_3_Montage'");
	if (GetHit3_Montage.Succeeded())
	{
		gethitMontage_3 = GetHit3_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> GetHit4_Montage(L"AnimMontage'/Game/ANIMATION/PlayerCharacterAnim/Hit_React_4_Montage.Hit_React_4_Montage'");
	if (GetHit4_Montage.Succeeded())
	{
		gethitMontage_4 = GetHit4_Montage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> GetHitPistol_Montage(L"AnimMontage'/Game/ANIMATION/PlayerCharacterAnim/Pistol_HitReaction1_Montage.Pistol_HitReaction1_Montage'");
	if (GetHitPistol_Montage.Succeeded())
	{
		gethitPistolMontage = GetHitPistol_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> GetHitPistol2_Montage(L"AnimMontage'/Game/ANIMATION/PlayerCharacterAnim/Pistol_HitReaction2_Montage.Pistol_HitReaction2_Montage'");
	if (GetHitPistol2_Montage.Succeeded())
	{
		gethitPistolMontage_2 = GetHitPistol2_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> GetStandHit1_Montage(L"AnimMontage'/Game/ANIMATION/AnimList/AnimEventDMG/StandHit1_Common_Montage.StandHit1_Common_Montage'");
	if (GetStandHit1_Montage.Succeeded())
	{
		standhitMontage_1 = GetStandHit1_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> GetStandHit2_Montage(L"AnimMontage'/Game/ANIMATION/AnimList/AnimEventDMG/StandHit2_Common_Montage.StandHit2_Common_Montage'");
	if (GetStandHit2_Montage.Succeeded())
	{
		standhitMontage_2 = GetStandHit2_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> GetStandHit3_Montage(L"AnimMontage'/Game/ANIMATION/AnimList/AnimEventDMG/StandHit3_Common_Montage.StandHit3_Common_Montage'");
	if (GetStandHit3_Montage.Succeeded())
	{
		standhitMontage_3 = GetStandHit3_Montage.Object;
	}
	// skill

	static ConstructorHelpers::FObjectFinder<UAnimMontage> GetStandReady_Skill(L"AnimMontage'/Game/ANIMATION/AnimList/AnimExplore/StandReady_Common_Montage_Skill.StandReady_Common_Montage_Skill'");
	if (GetStandReady_Skill.Succeeded())
	{
		standReady_Skill = GetStandReady_Skill.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> GetStandFiring_Skill(L"AnimMontage'/Game/ANIMATION/AnimList/AnimExplore/StandFiring_Common_Montage_Skill.StandFiring_Common_Montage_Skill'");
	if (GetStandFiring_Skill.Succeeded())
	{
		stand_Skill_Firing = GetStandFiring_Skill.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> GetStandFire_Skill(L"AnimMontage'/Game/ANIMATION/AnimList/AnimExplore/StandFire_Common_Montage_Skill.StandFire_Common_Montage_Skill'");
	if (GetStandFire_Skill.Succeeded())
	{
		stand_Skill_Fire = GetStandFire_Skill.Object;
	}


	// Death

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Death_Montage(L"AnimMontage'/Game/ANIMATION/PlayerCharacterAnim/Death_1_Montage.Death_1_Montage'");
	if(Death_Montage.Succeeded())
	{
		deathMontage_1 = Death_Montage.Object;
	}	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Death2_Montage(L"AnimMontage'/Game/ANIMATION/PlayerCharacterAnim/Death_2_Montage.Death_2_Montage'");
	if (Death2_Montage.Succeeded())
	{
		deathMontage_2 = Death2_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Death3_Montage(L"AnimMontage'/Game/ANIMATION/PlayerCharacterAnim/Death_3_Montage.Death_3_Montage'");
	if (Death3_Montage.Succeeded())
	{
		deathMontage_3 = Death3_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathPistol_Montage(L"AnimMontage'/Game/ANIMATION/PlayerCharacterAnim/Death_Ironsights_1_Montage.Death_Ironsights_1_Montage'");
	if (DeathPistol_Montage.Succeeded())
	{
		deathMontage_Pistol1 = DeathPistol_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathPistol2_Montage(L"AnimMontage'/Game/ANIMATION/PlayerCharacterAnim/Death_Ironsights_2_Montage.Death_Ironsights_2_Montage'");
	if (DeathPistol2_Montage.Succeeded())
	{
		deathMontage_Pistol2 = DeathPistol2_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> CrouchDeath1_Montage(L"AnimMontage'/Game/ANIMATION/AnimList/AnimBattle/CrouchDeath1_Common_Montage.CrouchDeath1_Common_Montage'");
	if (CrouchDeath1_Montage.Succeeded())
	{
		crouchdeathMontage_1 = CrouchDeath1_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> CrouchDeath2_Montage(L"AnimMontage'/Game/ANIMATION/AnimList/AnimBattle/CrouchDeath2_Common_Montage.CrouchDeath2_Common_Montage'");
	if (CrouchDeath2_Montage.Succeeded())
	{
		crouchdeathMontage_2 = CrouchDeath2_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> CrouchDeath3_Montage(L"AnimMontage'/Game/ANIMATION/AnimList/AnimBattle/CrouchDeath3_Common_Montage.CrouchDeath3_Common_Montage'");
	if (CrouchDeath3_Montage.Succeeded())
	{
		crouchdeathMontage_3 = CrouchDeath3_Montage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> StandDeath1_Montage(L"AnimMontage'/Game/ANIMATION/AnimList/AnimEventDMG/StandDeath1_Common_Montage.StandDeath1_Common_Montage'");
	if (StandDeath1_Montage.Succeeded())
	{
		standDeathMontage_1 = StandDeath1_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> StandDeath2_Montage(L"AnimMontage'/Game/ANIMATION/AnimList/AnimEventDMG/StandDeath2_Common_Montage.StandDeath2_Common_Montage'");
	if (StandDeath2_Montage.Succeeded())
	{
		standDeathMontage_2 = StandDeath2_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> StandDeath3_Montage(L"AnimMontage'/Game/ANIMATION/AnimList/AnimEventDMG/StandDeath3_Common_Montage.StandDeath3_Common_Montage'");
	if (StandDeath3_Montage.Succeeded())
	{
		standDeathMontage_3 = StandDeath3_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> StandDeath4_Montage(L"AnimMontage'/Game/ANIMATION/AnimList/AnimEventDMG/StandDeath4_Common_Montage.StandDeath4_Common_Montage'");
	if (StandDeath4_Montage.Succeeded())
	{
		standDeathMontage_4 = StandDeath4_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> StandDeath5_Montage(L"AnimMontage'/Game/ANIMATION/AnimList/AnimEventDMG/StandDeath5_Common_Montage.StandDeath5_Common_Montage'");
	if (StandDeath5_Montage.Succeeded())
	{
		standDeathMontage_5 = StandDeath5_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> StandDeath6_Montage(L"AnimMontage'/Game/ANIMATION/AnimList/AnimEventDMG/StandDeath6_Common_Montage.StandDeath6_Common_Montage'");
	if (StandDeath6_Montage.Succeeded())
	{
		standDeathMontage_6 = StandDeath6_Montage.Object;
	}




	static ConstructorHelpers::FObjectFinder<UAnimMontage> Aim_Montage(L"AnimMontage'/Game/ANIMATION/AnimList/AnimExplore/StandReady_Common_Montage.StandReady_Common_Montage'");
	if (Aim_Montage.Succeeded())
	{
		aimingMontage = Aim_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AimPistol_Montage(L"AnimMontage'/Game/ANIMATION/PlayerCharacterAnim/Pistol_FireReady_Montage.Pistol_FireReady_Montage'");
	if (AimPistol_Montage.Succeeded())
	{
		aimingPistolMontage = AimPistol_Montage.Object;
	}


	static ConstructorHelpers::FObjectFinder<UAnimMontage> Cover_Montage(L"AnimMontage'/Game/ANIMATION/PlayerCharacterAnim/Stand_to_Crouch_Rifle_Hip_Montage.Stand_to_Crouch_Rifle_Hip_Montage'");
	if (Cover_Montage.Succeeded())
	{
		coverMontage = Cover_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> CoverPistol_Montage(L"AnimMontage'/Game/ANIMATION/PlayerCharacterAnim/Pistol_StandtoKneel_Montage.Pistol_StandtoKneel_Montage'");
	if (CoverPistol_Montage.Succeeded())
	{
		coverPistolMontage = CoverPistol_Montage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ReloadRifle_Montage(L"AnimMontage'/Game/ANIMATION/PlayerCharacterAnim/Reload_Rifle_Hip_Montage.Reload_Rifle_Hip_Montage'");
	if (ReloadRifle_Montage.Succeeded())
	{
		reloadRifle = ReloadRifle_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ReloadPistol_Montage(L"AnimMontage'/Game/ANIMATION/PlayerCharacterAnim/Pistol_Reload_Montage.Pistol_Reload_Montage'");
	if (ReloadPistol_Montage.Succeeded())
	{
		reloadPistol = ReloadPistol_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ReloadShotgun_Montage(L"AnimMontage'/Game/ANIMATION/PlayerCharacterAnim/Reload_Shotgun_Hip_Montage.Reload_Shotgun_Hip_Montage'");
	if (ReloadShotgun_Montage.Succeeded())
	{
		reloadShotgun = ReloadShotgun_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ReloadSniper_Montage(L"AnimMontage'/Game/ANIMATION/PlayerCharacterAnim/Reload_Rifle_Hip_Montage.Reload_Rifle_Hip_Montage'");
	if (ReloadSniper_Montage.Succeeded())
	{
		reloadSniper = ReloadSniper_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ReloadCrouch_Montage(L"AnimMontage'/Game/ANIMATION/AnimList/AnimBattle/CrouchReload_Common_Montage.CrouchReload_Common_Montage'");
	if (ReloadCrouch_Montage.Succeeded())
	{
		reloadCrouch = ReloadCrouch_Montage.Object;
	}


	static ConstructorHelpers::FObjectFinder<UAnimMontage> Enemy_Shot_Montage(L"AnimMontage'/Game/ANIMATION/EnemyAnim/StandFire_Common_Montage.StandFire_Common_Montage'");
	if (Enemy_Shot_Montage.Succeeded())
	{
		Enemy_shotMontage = Enemy_Shot_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Enemy_Shoting_Montage(L"AnimMontage'/Game/ANIMATION/EnemyAnim/StandFiring_Common_Montage.StandFiring_Common_Montage'");
	if (Enemy_Shoting_Montage.Succeeded())
	{
		Enemy_shotingMontage = Enemy_Shoting_Montage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Enemy_GetHit_Montage(L"AnimMontage'/Game/ANIMATION/EnemyAnim/CrouchHit_Common_Montage.CrouchHit_Common_Montage'");
	if (Enemy_GetHit_Montage.Succeeded())
	{
		Enemy_HitMontage = Enemy_GetHit_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Enemy_Death1_Montage(L"AnimMontage'/Game/ANIMATION/EnemyAnim/CrouchDeath1_Common_Montage.CrouchDeath1_Common_Montage'");
	if (Enemy_Death1_Montage.Succeeded())
	{
		Enemy_Death1Montage = Enemy_Death1_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Enemy_Death2_Montage(L"AnimMontage'/Game/ANIMATION/EnemyAnim/CrouchDeath2_Common_Montage.CrouchDeath2_Common_Montage'");
	if (Enemy_Death2_Montage.Succeeded())
	{
		Enemy_Death2Montage = Enemy_Death2_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Enemy_Death3_Montage(L"AnimMontage'/Game/ANIMATION/EnemyAnim/CrouchDeath3_Common_Montage.CrouchDeath3_Common_Montage'");
	if (Enemy_Death3_Montage.Succeeded())
	{
		Enemy_Death3Montage = Enemy_Death3_Montage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Enemy_Aim_Montage(L"AnimMontage'/Game/ANIMATION/EnemyAnim/StandReady_Common_Montage.StandReady_Common_Montage'");
	if (Enemy_Aim_Montage.Succeeded())
	{
		Enemy_AimingMontage = Enemy_Aim_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Enemy_Cover_Montage(L"AnimMontage'/Game/Animation/EnemyCharacterAnim/Stand_to_Crouch_Rifle_Ironsights_Montage.Stand_to_Crouch_Rifle_Ironsights_Montage'");
	if (Enemy_Cover_Montage.Succeeded())
	{
		Enemy_BeCoverMontage = Enemy_Cover_Montage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> RightTurn_Montage(L"AnimMontage'/Game/ANIMATION/AnimList/AnimExplore/RunningRight_Common_Montage.RunningRight_Common_Montage'");
	if (RightTurn_Montage.Succeeded())
	{
		RightTurnMontage = RightTurn_Montage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Enemy_reload_Montage(L"	AnimMontage'/Game/ANIMATION/EnemyAnim/CrouchReload_Common_Montage.CrouchReload_Common_Montage'");
	if (Enemy_reload_Montage.Succeeded())
	{
		Enemy_ReloadMontage = Enemy_reload_Montage.Object;
	}


}

void UCharacterAnimInstance::BeShotRifle()
{
	Montage_Play(shotRifleMontage, 2.0f);
}
void UCharacterAnimInstance::BeShotPistol()
{
	Montage_Play(shotPistolMontage, 3.0f);
}
void UCharacterAnimInstance::BeShotShotgun()
{	
	Montage_Play(shotShotgunMontage, 1.0f);
}
void UCharacterAnimInstance::BeShotSniper()
{	
	Montage_Play(shotSniperMontage, 0.5f, EMontagePlayReturnType::MontageLength, 0.35f);
}

void UCharacterAnimInstance::BeCover()
{
	Montage_Play(coverMontage, 1.f);
}
void UCharacterAnimInstance::BeCoverPistol()
{
	Montage_Play(coverPistolMontage, 1.f);
}

void UCharacterAnimInstance::Hit()
{
	Montage_Play(gethitMontage, 2.0f);	
}
void UCharacterAnimInstance::Hit_2()
{
	Montage_Play(gethitMontage_2, 2.0f);
}
void UCharacterAnimInstance::Hit_3()
{
	Montage_Play(gethitMontage_3, 2.0f);
}
void UCharacterAnimInstance::Hit_4()
{
	Montage_Play(gethitMontage_4, 2.0f);
}
void UCharacterAnimInstance::HitPistol()
{
	Montage_Play(gethitPistolMontage, 2.f);
}
void UCharacterAnimInstance::HitPistol_2()
{
	Montage_Play(gethitPistolMontage_2, 2.f);
}


void UCharacterAnimInstance::Play_StandHit_1()
{
	Montage_Play(standhitMontage_1, 1.f);
}
void UCharacterAnimInstance::Play_StandHit_2()
{
	Montage_Play(standhitMontage_2, 1.f);
}
void UCharacterAnimInstance::Play_StandHit_3()
{
	Montage_Play(standhitMontage_3, 1.f);
}

void UCharacterAnimInstance::Play_SkillFiringMontage()
{
	Montage_Play(stand_Skill_Firing, 1.f);
}
void UCharacterAnimInstance::Play_SkillFireMontage()
{
	Montage_Play(stand_Skill_Fire, 1.f);
}


void UCharacterAnimInstance::Play_SkillReadyMontage()
{
	Montage_Play(standReady_Skill, 1.f);
}



void UCharacterAnimInstance::Death()
{
	Montage_Play(deathMontage_1, 1.0);
}
void UCharacterAnimInstance::Death_2()
{
	Montage_Play(deathMontage_2, 1.0);
}
void UCharacterAnimInstance::Death_3()
{
	Montage_Play(deathMontage_3, 1.0);
}
void UCharacterAnimInstance::Death_Pistol()
{
	Montage_Play(deathMontage_Pistol1, 1.0);
}
void UCharacterAnimInstance::Death_Pistol2()
{
	Montage_Play(deathMontage_Pistol2, 1.0);
}
void UCharacterAnimInstance::CrouchDeath_1()
{
	Montage_Play(crouchdeathMontage_1, 1.0);
}
void UCharacterAnimInstance::CrouchDeath_2()
{
	Montage_Play(crouchdeathMontage_2, 1.0);
}
void UCharacterAnimInstance::CrouchDeath_3()
{
	Montage_Play(crouchdeathMontage_3, 1.0);
}
void UCharacterAnimInstance::StandDeath_1()
{
	Montage_Play(standDeathMontage_1, 1.0);
}
void UCharacterAnimInstance::StandDeath_2()
{
	Montage_Play(standDeathMontage_2, 1.0);
}
void UCharacterAnimInstance::StandDeath_3()
{
	Montage_Play(standDeathMontage_3, 1.0);
}
void UCharacterAnimInstance::StandDeath_4()
{
	Montage_Play(standDeathMontage_4, 1.0);
}
void UCharacterAnimInstance::StandDeath_5()
{
	Montage_Play(standDeathMontage_5, 1.0);
}
void UCharacterAnimInstance::StandDeath_6()
{
	Montage_Play(standDeathMontage_6, 1.0);
}



void UCharacterAnimInstance::Aiming()
{
	Montage_Play(aimingMontage, 1.0f);
}
void UCharacterAnimInstance::AimingPistol()
{
	Montage_Play(aimingPistolMontage, 1.0f);
}

void UCharacterAnimInstance::Reload_Rifle()
{
	Montage_Play(reloadRifle, 1.0f);
}
void UCharacterAnimInstance::Reload_Pistol()
{
	Montage_Play(reloadPistol, 1.0f);
}
void UCharacterAnimInstance::Reload_Shotgun()
{
	Montage_Play(reloadShotgun, 1.0f);
}
void UCharacterAnimInstance::Reload_Sniper()
{
	Montage_Play(reloadSniper, 1.0f);
}
void UCharacterAnimInstance::Reload_Crouch()
{
	Montage_Play(reloadCrouch, 1.0f);
}

void UCharacterAnimInstance::Enemy_BeShot()
{
	Montage_Play(Enemy_shotMontage, 0.7f);
}
void UCharacterAnimInstance::Enemy_BeShoting()
{
	Montage_Play(Enemy_shotingMontage, 1.f);
}

void UCharacterAnimInstance::Enemy_BeCover()
{
	Montage_Play(Enemy_BeCoverMontage, 1.0f);
}

void UCharacterAnimInstance::Enemy_Hit()
{
	Montage_Play(Enemy_HitMontage, 2.0f);
}

void UCharacterAnimInstance::Enemy_Death1()
{
	Montage_Play(Enemy_Death1Montage, 1.f);
}
void UCharacterAnimInstance::Enemy_Death2()
{
	Montage_Play(Enemy_Death2Montage, 1.f);
}
void UCharacterAnimInstance::Enemy_Death3()
{
	Montage_Play(Enemy_Death3Montage, 1.f);
}

void UCharacterAnimInstance::Enemy_Aiming()
{
	Montage_Play(Enemy_AimingMontage, 1.0f);
}

void UCharacterAnimInstance::Enemy_Reloading()
{
	Montage_Play(Enemy_ReloadMontage, 1.0f);
}
void UCharacterAnimInstance::StopMontage()
{
	StopAllMontages(0.5f);
}

void UCharacterAnimInstance::SetWeaponNumber()
{
	//weaponNumber = 
	FString WeaponName = Cast<APlayerSquadCharacter>(GetOwningActor())->CharacterStat->GetCharacterWeaponName();

	if (WeaponName == "Rifle")
		weaponNumber = 0.f;
	else if (WeaponName == "Pistol")
		weaponNumber = 1.f;
	else if (WeaponName == "Shotgun")
		weaponNumber = 2.f;
	else if (WeaponName == "Sniper")
		weaponNumber = 3.f;
}

float UCharacterAnimInstance::GetWeaponNumber()
{
		return weaponNumber;
}

void UCharacterAnimInstance::SetAnimWeaponNumber_Implementation()
{
}

float UCharacterAnimInstance::floatTestFunc_Implementation()
{
	return 0.0f;
}

void UCharacterAnimInstance::Play_RightTurnMontage()
{
	Montage_Play(RightTurnMontage);
}

void UCharacterAnimInstance::Set_IsSpreadOut(bool IsSpreadOut)
{
	this->IsSpreadOut = IsSpreadOut;
}

bool UCharacterAnimInstance::GetIsSpreadOut()
{
	return IsSpreadOut;
}

void UCharacterAnimInstance::Call_GetIsSpreadOut_Implementation()
{

}

void UCharacterAnimInstance::Call_GetIsBattle_Implementation()
{

}

void UCharacterAnimInstance::Call_GetSpreadOutDirection_Implementation()
{

}

bool UCharacterAnimInstance::GetIsBattle()
{
	return IsStartBattle;
}

void UCharacterAnimInstance::Set_IsBattle(bool IsBattle)
{
	this->IsStartBattle = IsBattle;
}
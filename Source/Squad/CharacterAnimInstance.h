// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SQUAD_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	UCharacterAnimInstance();
	
	// virtual void NativeBeginPlay() override;
	// virtual void NativeUpdateAnimation(float DeltaSeconds);

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsBattle;

	// Shot
	UFUNCTION()
	void BeShotRifle();
	UAnimMontage* shotRifleMontage;
	void BeShotPistol();
	UAnimMontage* shotPistolMontage;
	void BeShotShotgun();
	UAnimMontage* shotShotgunMontage;
	void BeShotSniper();
	UAnimMontage* shotSniperMontage;

	// Hit - Battle 234 삭제 예정
	void Hit();
	void Hit_2();
	void Hit_3();
	void Hit_4();
	UAnimMontage* gethitMontage;
	UAnimMontage* gethitMontage_2;
	UAnimMontage* gethitMontage_3;
	UAnimMontage* gethitMontage_4;

	// Hit - Pistol
	void HitPistol();
	void HitPistol_2();
	UAnimMontage* gethitPistolMontage;
	UAnimMontage* gethitPistolMontage_2;

	// Hit - stand
	void Play_StandHit_1();
	void Play_StandHit_2();
	void Play_StandHit_3();
	
	UAnimMontage* standhitMontage_1;
	UAnimMontage* standhitMontage_2;
	UAnimMontage* standhitMontage_3;

	// skill
	void Play_SkillReadyMontage();

	UAnimMontage* standReady_Skill;

	void Play_SkillFiringMontage();

	UAnimMontage* stand_Skill_Firing;

	void Play_SkillFireMontage();

	UAnimMontage* stand_Skill_Fire;
	// Death
	UFUNCTION()
	void Death();
	UFUNCTION()
		void Death_2();
	UFUNCTION()
		void Death_3();
	UAnimMontage* deathMontage_1;
	UAnimMontage* deathMontage_2;
	UAnimMontage* deathMontage_3;

	UFUNCTION()
		void Death_Pistol();
	UFUNCTION()
		void Death_Pistol2();
	UAnimMontage* deathMontage_Pistol1;
	UAnimMontage* deathMontage_Pistol2;

	UFUNCTION()
		void CrouchDeath_1();
	UFUNCTION()
		void CrouchDeath_2();
	UFUNCTION()
		void CrouchDeath_3();
	UAnimMontage* crouchdeathMontage_1;
	UAnimMontage* crouchdeathMontage_2;
	UAnimMontage* crouchdeathMontage_3;

	UFUNCTION()
		void StandDeath_1();
	UFUNCTION()
		void StandDeath_2();
	UFUNCTION()
		void StandDeath_3();
	UFUNCTION()
		void StandDeath_4();
	UFUNCTION()
		void StandDeath_5();
	UFUNCTION()
		void StandDeath_6();
	UAnimMontage* standDeathMontage_1;
	UAnimMontage* standDeathMontage_2;
	UAnimMontage* standDeathMontage_3;
	UAnimMontage* standDeathMontage_4;
	UAnimMontage* standDeathMontage_5;
	UAnimMontage* standDeathMontage_6;

	// Aiming
	UFUNCTION()
	void Aiming();
	UAnimMontage* aimingMontage;
	UFUNCTION()
		void AimingPistol();
	UAnimMontage* aimingPistolMontage;



	// Cover
	UFUNCTION()
	void BeCover();
	UAnimMontage* coverMontage;
	UFUNCTION()
		void BeCoverPistol();
	UAnimMontage* coverPistolMontage;

	// Reload
	UFUNCTION()
		void Reload_Rifle();
	UAnimMontage* reloadRifle;
	UFUNCTION()
		void Reload_Pistol();
	UAnimMontage* reloadPistol;
	UFUNCTION()
		void Reload_Shotgun();
	UAnimMontage* reloadShotgun;
	UFUNCTION()
		void Reload_Sniper();
	UAnimMontage* reloadSniper;
	UFUNCTION()
		void Reload_Crouch();
	UAnimMontage* reloadCrouch;

	// Enemy
	UFUNCTION()
		void Enemy_BeShot();
	UAnimMontage* Enemy_shotMontage;
	UFUNCTION()
		void Enemy_BeShoting();
	UAnimMontage* Enemy_shotingMontage;
	UFUNCTION()
		void Enemy_BeCover();
	UAnimMontage* Enemy_BeCoverMontage;
	UFUNCTION()
		void Enemy_Hit();
	UAnimMontage* Enemy_HitMontage;
	UFUNCTION()
		void Enemy_Death1();
	UAnimMontage* Enemy_Death1Montage;
	UFUNCTION()
		void Enemy_Death2();
	UAnimMontage* Enemy_Death2Montage;
	UFUNCTION()
		void Enemy_Death3();
	UAnimMontage* Enemy_Death3Montage;
	UFUNCTION()
		void Enemy_Aiming();
	UAnimMontage* Enemy_AimingMontage;
	UFUNCTION()
		void Enemy_Reloading();
	UAnimMontage* Enemy_ReloadMontage;


	UFUNCTION()
		void Play_RightTurnMontage();
	UAnimMontage* RightTurnMontage;

	void StopMontage();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float weaponNumber;

	UFUNCTION(BlueprintCallable)
	void SetWeaponNumber();

	UFUNCTION(BlueprintPure)
		float GetWeaponNumber();

	UFUNCTION(BlueprintNativeEvent)
		void SetAnimWeaponNumber();
	virtual void SetAnimWeaponNumber_Implementation();



	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		float floatTestFunc();
	virtual float floatTestFunc_Implementation();




	UFUNCTION(BlueprintNativeEvent)
		void Call_GetIsSpreadOut();
	virtual void Call_GetIsSpreadOut_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void Call_GetSpreadOutDirection();
	virtual void Call_GetSpreadOutDirection_Implementation();

	void Set_IsSpreadOut(bool IsSpreadOut);

	UFUNCTION(BlueprintPure)
		bool GetIsSpreadOut();

	bool IsSpreadOut = false;


	UFUNCTION(BlueprintNativeEvent)
		void Call_GetIsBattle();
	virtual void Call_GetIsBattle_Implementation();

	void Set_IsBattle(bool IsSpreadOut);

	UFUNCTION(BlueprintPure)
		bool GetIsBattle();

	bool IsStartBattle = false;
};

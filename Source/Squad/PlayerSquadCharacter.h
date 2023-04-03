// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SquadCharacter.h"
//#include "EnemySquadCharacter.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundBase.h"
//#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "StatusBarWidget.h"
#include "PlayerSquadCharacter.generated.h"

////////////////////////////////////////////////////////////////
//					ClassNumer Brunch			              //
//														      //
//						0 = RifleMan	 			          //
//						1 = Deminer		 	 				  //
//						2 = NULL				              //
//						3 = Police			                  //
//						4 = medic			                  //
//						5 = assault		                      //
//						6 = ranger		                      //
//						7 = Sniper						      //
//						8 = scout							  //
//													          //
////////////////////////////////////////////////////////////////
/**
 * 
 */
UCLASS()
class SQUAD_API APlayerSquadCharacter : public ASquadCharacter
{
	GENERATED_BODY()

	APlayerSquadCharacter();

private:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	

public:	

	void InitCharacterStat();

	void SetSkeletalMeshComp(USkeletalMesh* Head, USkeletalMesh* Cap, USkeletalMesh* Cap_equip
							, USkeletalMesh* Head_equip1 , USkeletalMesh* Head_equip2, USkeletalMesh* Shirt
							, USkeletalMesh* Pants, USkeletalMesh* Boots, USkeletalMesh* Hands, USkeletalMesh* Vest
							, USkeletalMesh* Vest_outfit , USkeletalMesh* Backpack, USkeletalMesh* Backpack2, USkeletalMesh* Vest_collar
							, USkeletalMesh* Vest_shoulder_R, USkeletalMesh* Vest_shoulder_L , USkeletalMesh* Vest_bottom , USkeletalMesh* Decals
							, USkeletalMesh* Radio, USkeletalMesh* Kneepad_R, USkeletalMesh* Kneepad_L, USkeletalMesh* Holster);

public:	 

	void SetShotReady();
	void Debug_Shot(ASquadCharacter* Target);

	UFUNCTION(BlueprintCallable)
	void BeShot();

	void SetMoveReady();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void PlayerDeath(class UCharacterAnimInstance* CharAnimInst);

	void SetCoverReady();

	void SetCover();

	void SetStay();
	void SetSkill1();
	void SetSkill2();


	UFUNCTION(BlueprintCallable, Category = "TurnSystem")
	void SetCharacterEnd();
	
	void PlaySelectedSound();

	void InputTest();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Numbering")
	int32 numbering;

	void SetUnderGrid(class AGrid* Grid);
	class AGrid* GetUnderGrid();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Numbering")
	int32 ArrayNumbering;
	
	int32 GetBattleLineNumber();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSetting")
	int32 BattleLineNumber;

	// 버프 시스템 초기

	void Buff_System();
	void Buff_Cover(bool OnOff);

	UFUNCTION(BlueprintCallable)
	void BeShowMouseCursor();
	UFUNCTION(BlueprintCallable)
	void BeHideMouseCursor();

	bool IsActiveBuffCover = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSetting")
	float CharacterDefenceArmor = 0.f;

	void StopMontage();

	int32 ClassNum;

	//

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSetting")
	bool tempCharacterTest = false;


protected:

	// 오디오 정리 필요
	/*
	UAudioComponent* AudioComp;
	UAudioComponent* AudioComp_Hit;
	*/
	USoundBase* Fire_Sound;

	USoundBase* GetHit_Sound;
	USoundBase* Death_Sound;

	USoundBase* Selected_Sound;

	// 총기 오디오 
	
	ASquadCharacter* tempTargetCharacter = nullptr;

public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent*	Cap;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent*	Cap_equip;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent*	Head_equip1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent*	Head_equip2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent*	Shirt;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent*	Pants;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent*	Boots;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent*	Hands;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent*	Vest;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent*	Vest_outfit;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent*	Backpack;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent*	Backpack2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent*	Vest_Collar;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent*	Vest_shoulder_R;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent*	Vest_shoulder_L;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent*	Vest_Bottom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent*	Decals;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent*	Radio;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent*	Kneepad_R;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent*	Kneepad_L;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent*	Holster;


	void SetFXSound(const TCHAR* HitSoundContentPath, const TCHAR* DeadSoundConentPath);
	void SetContentMesh(USkeletalMeshComponent* mesh, const TCHAR* ContentPath);
	void SetContentMeshMat(USkeletalMeshComponent * mesh, const TCHAR * ContentPath, int32 MatIndex = 0);
	
	void SetWeaponMesh();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	class USkeletalMeshComponent* WeaponSlot;

	public:

	//UPROPERTY(EditAnywhere)
	//	float CurrentAmmo;

		void SetReloadReady();

		void BeReload();

		void BeReload_BattleOver();

		void Calc_Damage_distribution(ASquadCharacter* TargetEvasionCorrection);
		TArray<float> Damage_distribution;
		TArray<float> Damage_distribution_float;
		float MaxDamage_InDamageDis = 0.f;

		int factorial(int n);

		void Calc_SkillDamage_distribution(ASquadCharacter* Target, struct FSkillValueList* CompSkillData);
		TArray<float> SkillDamage_distribution;
		TArray<float> SkillDamage_distribution_float;
		float SkillMaxDamage_InDamageDis = 0.f;

	//

		UFUNCTION()
			void SetHighLight(bool OnOff);

		void SetHighLight_SelfSkill(bool OnOff);

		UFUNCTION()
			void SetTurnOnHighLightGrid();
		UFUNCTION()
			void SetTurnOffHighLightGrid();
	
		public:

			class USquadCharacterSkillComponent* CharacterSkillComp;


			UFUNCTION()
				void SetPlayerSkill_ClassNum(int32 ClassNum);

			void SetSkillNumAndTarget(int32 skillNum, AActor * TargetCharacter);
			int32 skillNum = 0;
			AActor* SkillTargetCharacter;

			UFUNCTION(BlueprintCallable)
				void UsePlayerSkill(int32 skillNum, AActor* TargetCharacter);


			void DebugMessage_CharacterState();
			bool IsCharacterUseAttack = false;

			UFUNCTION(BlueprintCallable)
				void SetIsCharacterUseAttackTotrue();

			UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
			bool SpreadOutDirection= true; // true면 왼쪽 false 오른쪽
};

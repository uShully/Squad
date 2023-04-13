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

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void PlayerDeath(class UCharacterAnimInstance* CharAnimInst);

	// 캐릭터 상태 변환
	void SetCoverReady();
	void SetShotReady();
	void SetReloadReady();
	void SetStayReady();

	// 행동 초기화 및 애니메이션 재생
	void SetShot(ASquadCharacter* Target);
	void SetSkill1();
	void SetSkill2();
	void SetCover();	   

	// 전투 시 재장전
	void BeReload();
	// 전투 종료시 재장전
	void BeReload_BattleOver();
	// 일반 사격 데미지 계산함수
	UFUNCTION(BlueprintCallable)
		void BeShot();

	// 사격,스킬 데미지 이항분포 계산 , 확률 배열에 저장
	void Calc_Damage_distribution(ASquadCharacter* TargetEvasionCorrection);
	TArray<float> Damage_distribution; // 데미지 판별용
	TArray<float> Damage_distribution_float; // 데미지 확률표 표기용

	void Calc_SkillDamage_distribution(ASquadCharacter* Target, struct FSkillValueList* CompSkillData);
	TArray<float> SkillDamage_distribution;
	TArray<float> SkillDamage_distribution_float;

	// 이항분포 계산식 변수
	float SkillMaxDamage_InDamageDis = 0.f;
	float MaxDamage_InDamageDis = 0.f;

private:
	// 팩토리얼 함수 //
	int factorial(int n);

public:

	UFUNCTION(BlueprintCallable, Category = "TurnSystem")
	void SetCharacterEnd();
	
	void PlaySelectedSound();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Numbering")
	int32 numbering;

	void SetUnderGrid(class AGrid* Grid);
	class AGrid* GetUnderGrid();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Numbering")
	int32 ArrayNumbering;
	
	int32 GetBattleLineNumber();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSetting")
	int32 BattleLineNumber;

	int32 ClassNum;
	/////////////////////////

	// 버프 시스템 초기버전
	// 0.10.14 - 다음 업데이트시 변경예정
	void Buff_System();
	void Buff_Cover(bool OnOff);

	bool IsActiveBuffCover = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSetting")
		float CharacterDefenceArmor = 0.f;

	// 마우스 활성화 비활성화
	UFUNCTION(BlueprintCallable)
	void BeShowMouseCursor();
	UFUNCTION(BlueprintCallable)
	void BeHideMouseCursor();
	   	 
	// 애님 몽타주 종료
	void StopMontage();




protected:

	// 오디오 
	USoundBase* Fire_Sound;
	USoundBase* GetHit_Sound;
	USoundBase* Death_Sound;
	USoundBase* Selected_Sound;	
	
	ASquadCharacter* tempTargetCharacter = nullptr;

public:
	
	// 메쉬
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
		class USkeletalMeshComponent* WeaponSlot;
	/////////////////////////

	// 매쉬, 사운드 동적할당
	void SetFXSound(const TCHAR* HitSoundContentPath, const TCHAR* DeadSoundConentPath);
	void SetContentMesh(USkeletalMeshComponent* mesh, const TCHAR* ContentPath);
	void SetContentMeshMat(USkeletalMeshComponent * mesh, const TCHAR * ContentPath, int32 MatIndex = 0);
	void SetWeaponMesh();
	/////////////////////////
	   	  
public:

	// 외곽선 강조 함수 //
	UFUNCTION()
		void SetHighLight(bool OnOff);

	void SetHighLight_SelfSkill(bool OnOff);

	UFUNCTION()
		void SetTurnOffHighLightGrid();

	bool GetbIsHighLight() { return bIsHighLight; };
	bool GetbIsSelfHighLight() { return bIsSelfHighLight; };

private:

	bool bIsHighLight = false;
	void SetbIsHighLight(bool OnOff) { bIsHighLight = OnOff; };

	bool bIsSelfHighLight = false;
	void SetbIsSelfHighLight(bool OnOff) { bIsSelfHighLight = OnOff; };

	/////////////////////////

public:
	// 스킬 타겟 및 사용 함수 //
	class USquadCharacterSkillComponent* CharacterSkillComp;


	UFUNCTION()
		void SetPlayerSkill_ClassNum(int32 ClassNum);


	UFUNCTION(BlueprintCallable)
		void UsePlayerSkill(int32 skillNum, AActor* TargetCharacter);

	void SetSkillNumAndTarget(int32 skillNum, AActor * TargetCharacter);
	int32 skillNum = 0;
	AActor* SkillTargetCharacter;

	/////////////////////////

	// AnimBlueprint 제어 함수 및 변수 //
	UFUNCTION(BlueprintCallable)
		void SetIsCharacterUseAttackTotrue();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool SpreadOutDirection= true; // true면 왼쪽 false 오른쪽	

	bool IsCharacterUseAttack = false;

	/////////////////////////
};

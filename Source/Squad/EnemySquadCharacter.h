// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SquadCharacter.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundBase.h"
//#include "Kismet/GameplayStatics.h"
//#include "UObject/ConstructorHelpers.h"
#include "EnemySquadCharacter.generated.h"

DECLARE_DELEGATE(FDeleEnemy_Single);

UENUM(BlueprintType)
enum class EEnemyRate : uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Boss UMETA(DisplayName = "Boss"),
};
/**
 * 
 */
UCLASS()
class SQUAD_API AEnemySquadCharacter : public ASquadCharacter
{
	GENERATED_BODY()

	AEnemySquadCharacter();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:
	
	FDeleEnemy_Single Fun_Death;
	void SetBelongToBattleTrigger(class ABattleTrigger* BattleTigger);
	ABattleTrigger* GetBelongToBattleTrigger();

	void Enemy_ReadytoShot(class APlayerSquadCharacter* shotTarget);
	class APlayerSquadCharacter* tempshotTarget;

	UFUNCTION(BlueprintCallable)
	void Enemy_Shot(AActor* Target);
	UFUNCTION(BlueprintCallable)
	void Enemy_TurnEnd();

	void CharacterSkill_Reload();

	void SetUnderGrid(class AGrid* Grid);
	class AGrid* GetUnderGrid();

	void SetWeaponStat(FString WeaponName);

	void SetSkeletalMesh(const TCHAR* ContentPath);

	void SetWeaponMesh(const TCHAR * ContentPath);

	void SetSkeletalMeshPath(int32 BrunchNum);

	UFUNCTION()
	void SetBrunchAnimBP();

protected:

	USoundBase* Fire_Sound;
	USoundBase* GetHit_Sound;
	USoundBase* Death_Sound;

	void EnemyDeath(class UCharacterAnimInstance* CharAnimInst);

	class ABattleTrigger* BelongToBattleTrigger;

public:

	FTimerHandle WaitHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSetting")
	float WaitTime;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "GameSetting")
	int32 ArrayNumbering;

	AActor* ShotTarget;
	void SetShotTarget(AActor* Target) { this->ShotTarget = Target;  };

protected:

	UPROPERTY()
		TSubclassOf<class UAnimInstance> RecruitAnimBP;

	UPROPERTY()
		TSubclassOf<class UAnimInstance> RifleAnimBP;

	UPROPERTY()
		TSubclassOf<class UAnimInstance> PistolAnimBP;

	UPROPERTY()
		TSubclassOf<class UAnimInstance> ShotgunAnimBP;

	UPROPERTY()
		TSubclassOf<class UAnimInstance> SniperAnimBP;


public:
	FText EnemyCharacter_BrunchName;
	FText GetEnemyCharacterBrunchName() { return EnemyCharacter_BrunchName; }

	class USkeletalMesh* CharacterMesh;
	int32 weaponNum;

	UFUNCTION()
		void SetHighLight(bool OnOff);

	UPROPERTY(EditAnywhere, Category = "Enemy_State")
		EEnemyRate EnemyRate = EEnemyRate::Normal;

	bool IsCharacterUseAttack = false;
	class ASquadCharacter* tempTargetCharacter;

	private:

		bool bIsHighLight = false;
		void SetbIsHighLight(bool OnOff) { bIsHighLight = OnOff; };

	public:
		bool GetbIsHighLight() { return bIsHighLight; };
};

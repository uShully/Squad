// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Squad.h"
#include "GameFramework/PlayerController.h"
#include "CursorHighlight.h"
#include "BattleController.h"
#include "Components/TimelineComponent.h"
#include "SquadCharacter.h"
#include "EnemySquadCharacter.h"
#include "PlayerSquadCharacter.h"
#include "SquadCameraManager.h"
#include "SquadController.generated.h"

/**
 * 
 */
UCLASS()
class SQUAD_API ASquadController : public APlayerController
{
	GENERATED_BODY()
	
	ASquadController();

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UFUNCTION()
		void RayHit();

private:

	

	UPROPERTY(Visibleinstanceonly, BlueprintReadOnly, Category = "Battle", meta = (AllowPrivateAccess = "true"))
		class ABattleController* BattleController;

	UPROPERTY(Visibleinstanceonly, BlueprintReadOnly, Category = "Battle", meta = (AllowPrivateAccess = "true"))
		class USquadGameInstance* gameIns;

	UPROPERTY(Visibleinstanceonly, BlueprintReadOnly, Category = "Battle", meta = (AllowPrivateAccess = "true"))
		class ASquadGameMode* SquadGameMode;

	FHitResult Hit;
	FVector HitActorLoc;		   

	class ASquadCharacter* pRayHitCharacter;
	class AEnemySquadCharacter* pRayHitEnemyCharacter;
	class ASquadCharacter* pRayHitSelectedCharacter;
	class ASquadCharacter* Controller_SelectedCharacter;

	class APlayerSquadCharacter* RayHitCharacter;

	FHitResult RayHitResult;

public:

	void SetpRayHitCharacter(ASquadCharacter* Char) { pRayHitCharacter = Char; };
	void SetpRayHitSelectedCharacter(ASquadCharacter* Char) { pRayHitSelectedCharacter = Char; };

	
	// Key Bind 

	void SetKeyBindSkillButton1();
	void SetKeyBindSkillButton2();
	void SetKeyBindSkillButton3();
	void SetKeyBindSkillButton4();
	void SetKeyBindSkillButton5();

	void SetkeyBindMousewheel();
	void SetkeyBindMousewheelreverse();

	void ControlTarget_ExplorerWheel();
	void ControlTarget_ExplorerWheelReverse();
	//

	FHitResult HitTarget;

	void SetTargetCharacter();
	void ClearpRayHitCharacterValue();
	void SetTargetCharacter_Explorer();

	class APlayerSquadCharacter* Target_Explorer = nullptr;
	class APlayerSquadCharacter* preTarget_Explorer = nullptr;


	void EmptypRayHitCharacter(ASquadCharacter* CurrentChar);

	bool CompareInputActionBindings(FInputActionBinding lhs, FInputActionBinding rhs);

	void ClearSelectedCharacter_Explorer();

	//UFUNCTION()
	void SetSquadControllerInput(bool bIsStop);

	void SetSelectedCharacterInfo();

	UFUNCTION(BlueprintCallable)
	void SetMenuInput(bool bIsStop);

	void SetBattleController();

private:

	float YOffset;

	UPROPERTY()
		int32 IsGamePlay = 0;

	UFUNCTION()
		int32 SwitchGamePlayValue();

	class USoundBase* Selected_Sound;

	
};
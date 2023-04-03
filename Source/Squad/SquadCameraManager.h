// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SquadCharacter.h"
#include "Components/BoxComponent.h"
#include "SquadCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class SQUAD_API ASquadCameraManager : public ACharacter
{
	GENERATED_BODY()


	ASquadCameraManager();

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	
	void SortFrindlyCharList();
	//virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// 2.14
	void PlusTargetArmLeght();
	void MinusTargetArmLength();
	void PlusCameraBoomRotator();
	void MinusCameraBoomRotator();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void MoveRight(float Value);
	

	virtual void BeginPlay() override;

	void MoveChar(float Value);

	
	UFUNCTION()
	void GetFriendlyChar();

	TArray<AActor*> FriendlyCharList;



	//////////////////////////

	void ChangeArmLeght(float DeltaTime);
	void zoomswitch();
	void SetUnitMovement_Delay(int32 num);
	bool ZoomBool = false;

	bool MoveSwitch = false;

	

	///////////////////////// Temp Value /////////////////////


	void Control_PlayerCharacterMovement(bool Switch);
	bool ControlValue_PlayerCharacterMovement = true;
	bool ControlValue_PlayerCharacterMovement_BeginPlay = true;

	void Control_CameraMovement(bool Switch);
	bool ControlValue_CameraMovement = false;

	void Control_SetBattleInit(FVector Loc);

	void Control_SetExploreInit(FVector Loc);

	void Control_SetBattleCameraLocation(float DeltaTime);

	void Control_SetBattleEnd();

	void Control_ResultToRun();

	FVector ExplorerLocation;
	FVector BattleLocation;

	bool IsExploreToBattle = false;
	bool IsBattleToExplore = false;
	
	//////////////////////////////////////////////////////////


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent* UnitPos_First;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent* UnitPos_Last;

	UPROPERTY()
		bool UnitPos_Check_First = false;
	UPROPERTY()
		bool UnitPos_Check_Last = false;
	UPROPERTY()
		bool UnitPos_Init = false;


	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	

	UFUNCTION()
		void SetUnitPos_Last_Location(int32 number);

	


	FRotator ExploreRot;
	FRotator BattleRot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSetting")
		float ExplorePitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSetting")
		float ExploreYaw = -180.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSetting")
		float ExploreRoll  = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSetting")
		float BattlePitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSetting")
		float BattleYaw = -180.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSetting")
		float BattleRoll = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSetting")
		float MaxTargetArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSetting")
		float minTargetArmLength;

	//UPROPERTY()
	//	TSubclassOf<class APlayerSquadCharacter> Character_temp;
	
	FVector UnitPos_First_RelativeLocation;
	FVector UnitPos_Last_RelativeLocation;

	FVector EventSpotOverlapLocation_First;
	FVector EventSpotOverlapLocation_Last;
	void EventSpotOverlapLocation_Save();

	private:
		class ASquadController* SplayerController;

};

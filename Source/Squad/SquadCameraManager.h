// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Squad.h"
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

	void SortFrindlyCharList();

protected:
	// 디버그용 함수
	void PlusTargetArmLeght();
	void MinusTargetArmLength();
	void PlusCameraBoomRotator();
	void MinusCameraBoomRotator();

	// 캐릭터 이동
	UFUNCTION()
		void MoveRight(float Value);
	void MoveChar(float Value);

	void ChangeArmLeght(float DeltaTime);

	void EventSpotOverlapLocation_Save();


public:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void SetFrindlyCharList(TArray<AActor*> CharList);

	void zoomswitch();
	void SetUnitMovement_Delay(int32 num);

	void Control_CameraMovement(bool Switch);
	void Control_PlayerCharacterMovement(bool Switch);
	void Control_SetBattleInit(FVector Loc);
	void Control_SetExploreInit(FVector Loc);
	void Control_SetBattleCameraLocation(float DeltaTime);


	UFUNCTION()
		void SetUnitPos_Last_Location(int32 number);

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* SideViewCameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBoxComponent* UnitPos_First;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBoxComponent* UnitPos_Last;

	TArray<AActor*> FriendlyCharList;

	// 카메라, 캐릭터 제어 변수 
	bool ZoomBool = false;
	bool MoveSwitch = false;	

	bool ControlValue_PlayerCharacterMovement = true;
	bool ControlValue_PlayerCharacterMovement_BeginPlay = true;
	bool ControlValue_CameraMovement = false;

	bool IsExploreToBattle = false;
	bool IsBattleToExplore = false;

	UPROPERTY()
		bool UnitPos_Check_First = false;
	UPROPERTY()
		bool UnitPos_Check_Last = false;
	UPROPERTY()
		bool UnitPos_Init = false;

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

private:
	
		FVector ExplorerLocation;
		FVector BattleLocation;

		FVector UnitPos_First_RelativeLocation;
		FVector UnitPos_Last_RelativeLocation;

		FVector EventSpotOverlapLocation_First;
		FVector EventSpotOverlapLocation_Last;

		FRotator ExploreRot;
		FRotator BattleRot;

		class ASquadController* SplayerController;

};

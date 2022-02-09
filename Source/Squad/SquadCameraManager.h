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

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
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
	bool ZoomBool = false;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		float MaxTargetArmLength;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		float minTargetArmLength;

	///////////////////////// Temp Value /////////////////////


	void Control_PlayerCharacterMovement(bool TorF);
	bool ControlValue_PlayerCharacterMovement = true;

	void Control_CameraMovement();
	bool ControlValue_CameraMovement = false;

	void Control_SetBattleInit(FVector Loc);


	//////////////////////////////////////////////////////////


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent* BoxColiision;
};

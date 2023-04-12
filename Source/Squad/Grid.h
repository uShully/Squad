// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/StaticMeshComponent.h"
#include "GridManager.h"
#include "BattleTrigger.h"
#include "Grid.generated.h"

UENUM(BlueprintType)
enum class EGridState : uint8
{
	TE_OptionA UMETA(DisplayName = "Option A"),
	TE_OptionB UMETA(DisplayName = "Option B"),
	TE_OptionC UMETA(DisplayName = "Option C"),
};

UENUM(BlueprintType)
enum class EGridOntheObject : uint8
{
	Player UMETA(DisplayName = "Player"),
	Obstacle UMETA(DisplayName = "Obstacle"),
	Enemy UMETA(DisplayName = "Enemy"),
	Normal UMETA(DisplayName = "Normal"),
	Behind UMETA(DisplayName = "BehindObstacle")
};

UCLASS()
class SQUAD_API AGrid : public AActor
{
	GENERATED_BODY()

	AGrid();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Grid, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Plane;
public:
	struct GridInfomation {

		int32 XPos;
		int32 YPos;

		// HaveTurnCharacter�� SelectedCharacter�� ����
		AActor* GridOnTheCharacter;
		EGridOntheObject GOTO;

		void init() { XPos = 0; YPos = 0; GridOnTheCharacter = nullptr; }

	};
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Color, meta = (AllowPrivateAccess = "true"))
	UMaterial* WhiteMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Color, meta = (AllowPrivateAccess = "true"))
	UMaterial* RedMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Color, meta = (AllowPrivateAccess = "true"))
	UMaterial* BlueMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Color, meta = (AllowPrivateAccess = "true"))
	UMaterial* GreenMaterial;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Color, meta = (AllowPrivateAccess = "true"))
	UMaterial* BlackMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Color, meta = (AllowPrivateAccess = "true"))
	UMaterial* ObstacleMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Color, meta = (AllowPrivateAccess = "true"))
	UMaterial* 	TransparencyMaterial;
	
protected:

	virtual void BeginPlay() override;

	void SetGridState_Material();
	void SetGridInfo();
	
public:


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Grid, meta = (AllowPrivateAccess = "true"))
		int XPos;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Grid, meta = (AllowPrivateAccess = "true"))
		int YPos;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Grid, meta = (AllowPrivateAccess = "true"))
	bool RestrictGrid = false;
	
	UPROPERTY(EditAnywhere, Category = Grid, meta = (AllowPrivateAccess = "true"))
	EGridState GridState;


	GridInfomation GridInfo;

	class AGridManager* parentEventBox;
public:
 
	// GridManger 초기화용 함수
	void SetGridState();

	// Grid 색 변환 함수
	void SetGridInfo_Material_Init();
	void SetGridInfo_Material_InitInfo();
	void SetGridInfo_Material_Black();
	void SetGridInfo_Material_Red();
	void SetGridInfo_Material_Blue();
	void SetGridInfo_Material_Green();
	   
	UFUNCTION()
	void DeleteGrid();

	UFUNCTION()
	void SetVisible();

	UFUNCTION()
	void InitGrid();
	
};

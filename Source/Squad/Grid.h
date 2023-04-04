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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Grid, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Plane;

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

	/*

	UMaterialInstanceDynamic* ExampleMID = UMaterialInstanceDynamic::Create(UMaterialInterface* Exp, )
	ExampleStaticMesh->SetMaterial(ExampleMID, 0);  
	FLinearColor Red(FLineaerColor::Red);  
	ExampleMID->SetVectorParameter("ParameterName", Red);  
	*/



public:
	// Sets default values for this actor's properties
	AGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:

	//class UStaticMeshComponent* GetPlane() const { return Plane; }

		
	//

public:


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Grid, meta = (AllowPrivateAccess = "true"))
	int XPos;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Grid, meta = (AllowPrivateAccess = "true"))
	int YPos;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Grid, meta = (AllowPrivateAccess = "true"))
	bool RestrictGrid = false;
	
	UPROPERTY(EditAnywhere, Category = Grid, meta = (AllowPrivateAccess = "true"))
	EGridState GridState;

	struct GridInfomation {

		int32 XPos;
		int32 YPos;

		// HaveTurnCharacter�� SelectedCharacter�� ����
		AActor* GridOnTheCharacter;
		EGridOntheObject GOTO;

		void init() { XPos = 0; YPos = 0; GridOnTheCharacter = nullptr; }

	};

	
	GridInfomation GridInfo;

	void SetGridState();
	void SetGridInfo();

	void SetGridInfo_Material();
	//
	void SetGridInfo_Material_InitInfo();
	void SetGridInfo_Material_Black();
	void SetGridInfo_Material_Red();
	void SetGridInfo_Material_Blue();
	void SetGridInfo_Material_Green();
	//


	UFUNCTION()
	void DeleteGrid();

	UFUNCTION()
	void SetVisible();

	UFUNCTION()
	void InitGrid();

private:

	void SetGridState_Material();

public:

	class AGridManager* parentEventBox;

	
};

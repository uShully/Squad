// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	Plane->SetupAttachment(RootComponent);

	/*
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Plane_Body(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));
	if (Plane_Body.Succeeded())
	{
		Plane->SetStaticMesh(Plane_Body.Object);
	}
	*/
	
	static ConstructorHelpers::FObjectFinder<UMaterial> M_White(TEXT("Material'/Game/BLUEPRINT/GridColorIns/M_White.M_White'"));
	if (M_White.Object != NULL)
	{
		WhiteMaterial = (UMaterial*)M_White.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> M_Red(TEXT("Material'/Game/BLUEPRINT/GridColorIns/M_Red.M_Red'"));
	if (M_Red.Object != NULL)
	{
		RedMaterial = (UMaterial*)M_Red.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> M_Blue(TEXT("Material'/Game/BLUEPRINT/GridColorIns/M_Blue.M_Blue'"));
	if (M_Blue.Object != NULL)
	{
		BlueMaterial = (UMaterial*)M_Blue.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UMaterial> M_Green(TEXT("Material'/Game/BLUEPRINT/GridColorIns/M_Green.M_Green'"));
	if (M_Green.Object != NULL)
	{
		GreenMaterial = (UMaterial*)M_Green.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> M_Black(TEXT("Material'/Game/BLUEPRINT/GridColorIns/M_Black.M_Black'"));
	if (M_Black.Object != NULL)
	{
		BlackMaterial = (UMaterial*)M_Black.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UMaterial> M_Obstacle(TEXT("Material'/Game/BLUEPRINT/GridColorIns/M_ObstacleGrid.M_ObstacleGrid'"));
	if (M_Obstacle.Object != NULL)
	{
		ObstacleMaterial = (UMaterial*)M_Obstacle.Object;
	}
	
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();

	
}

void AGrid::SetGridState()
{
	SetGridState_Material();
	SetGridInfo();
}

void AGrid::SetGridState_Material()
{
	if(GridInfo.GOTO == EGridOntheObject::Normal)
	{
		if (GridState == EGridState::TE_OptionA)
		{
			Plane->SetMaterial(0, BlueMaterial);
		}
		else if (GridState == EGridState::TE_OptionB)
		{
			Plane->SetMaterial(0, WhiteMaterial);
		}
		else if (GridState == EGridState::TE_OptionC)
		{
			Plane->SetMaterial(0, RedMaterial);
		}
	}
}

void AGrid::SetGridInfo()
{
	GridInfo.init();
	GridInfo.XPos = this->XPos;
	GridInfo.YPos = this->YPos;
	SetGridInfo_Material_InitInfo();
}

void AGrid::SetGridInfo_Material()
{
	if (GridInfo.GOTO == EGridOntheObject::Normal)
	{
		Plane->SetMaterial(0, BlueMaterial);
	}
	else if (GridInfo.GOTO == EGridOntheObject::Player)
	{
		Plane->SetMaterial(0, GreenMaterial);
	}
	else if (GridInfo.GOTO == EGridOntheObject::Enemy)
	{
		Plane->SetMaterial(0, GreenMaterial);
	}
	else if (GridInfo.GOTO == EGridOntheObject::Obstacle)
	{
		Plane->SetMaterial(0, ObstacleMaterial);
	}
}

void AGrid::SetGridInfo_Material_InitInfo()
{
	if (GridInfo.GOTO == EGridOntheObject::Normal)
	{
		//Plane->SetMaterial(0, BlueMaterial);
	}
	else if (GridInfo.GOTO == EGridOntheObject::Player)
	{
		Plane->SetMaterial(0, GreenMaterial);
	}
	else if (GridInfo.GOTO == EGridOntheObject::Enemy)
	{
		Plane->SetMaterial(0, GreenMaterial);
	}
}

void AGrid::SetGridInfo_Material_Black()
{
	Plane->SetMaterial(0, BlackMaterial);
}

void AGrid::SetGridInfo_Material_Red()
{
	Plane->SetMaterial(0, RedMaterial);
}

void AGrid::SetGridInfo_Material_Blue()
{
	Plane->SetMaterial(0, BlueMaterial);
}

void AGrid::SetGridInfo_Material_Green()
{
	Plane->SetMaterial(0, GreenMaterial);
}

int AGrid::GetXPos()
{
	return XPos;
}

int AGrid::GetYPos()
{
	return YPos;
}

void AGrid::DeleteGrid()
{
	Destroy();
}

void AGrid::SetVisible()
{
	if (!RootComponent->IsVisible())
	RootComponent->SetVisibility(true);
	else
	RootComponent->SetVisibility(false);
}

void AGrid::InitGrid()
{
	FCoordinate InitCoor;

	InitCoor.pGrid = this;
	InitCoor.InitCoordinate(XPos, YPos);

	if (YPos == 0)
	{
		parentEventBox->parentBattleTrigger->Coordinate.Add(InitCoor);
		parentEventBox->parentBattleTrigger->Coordinate[XPos].MultiArray.Add(InitCoor);
	}
	else
	{
		parentEventBox->parentBattleTrigger->Coordinate[XPos].MultiArray.Add(InitCoor);		
	}
	parentEventBox->parentBattleTrigger->Coordinate[XPos].MultiArray[YPos].pGrid = this;

}
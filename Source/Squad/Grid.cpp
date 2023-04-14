// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AGrid::AGrid()
{
	PrimaryActorTick.bCanEverTick = false;

	Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	SetRootComponent(Plane);

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

	static ConstructorHelpers::FObjectFinder<UMaterial> M_TRANSPARENCY(TEXT("Material'/Game/BLUEPRINT/GridColorIns/M_transparency.M_transparency'"));
	if(M_TRANSPARENCY.Succeeded())
	{
		TransparencyMaterial = (UMaterial*)M_TRANSPARENCY.Object;
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

void AGrid::SetGridInfo_Material_Init()
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
		parentGridManager->parentBattleTrigger->Coordinate.Add(InitCoor);
		parentGridManager->parentBattleTrigger->Coordinate[XPos].MultiArray.Add(InitCoor);
	}
	else
	{
		parentGridManager->parentBattleTrigger->Coordinate[XPos].MultiArray.Add(InitCoor);		
	}
	parentGridManager->parentBattleTrigger->Coordinate[XPos].MultiArray[YPos].pGrid = this;

}
// Fill out your copyright notice in the Description page of Project Settings.

#include "GridComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values for this component's properties
UGridComponent::UGridComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PLANE"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Plane_Body(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));
	if (Plane_Body.Succeeded())
	{
		Plane->SetStaticMesh(Plane_Body.Object);
	}
}


// Called when the game starts
void UGridComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGridComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


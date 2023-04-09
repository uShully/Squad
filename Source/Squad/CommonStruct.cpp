// Fill out your copyright notice in the Description page of Project Settings.

#include "CommonStruct.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ACommonStruct::ACommonStruct()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Class_0_Mesh_Load();



}

// Called when the game starts or when spawned
void ACommonStruct::BeginPlay()
{
	Super::BeginPlay();
	
}


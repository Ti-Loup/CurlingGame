// Fill out your copyright notice in the Description page of Project Settings.


#include "RockRed.h"

// Sets default values
ARockRed::ARockRed()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/Mesh/RockRed"));
	if (MeshObj.Succeeded())
		RockMesh->SetStaticMesh(MeshObj.Object);
}

// Called when the game starts or when spawned
void ARockRed::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARockRed::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


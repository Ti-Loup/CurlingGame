// Fill out your copyright notice in the Description page of Project Settings.


#include "RockBlue.h"

// Sets default values
ARockBlue::ARockBlue()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/Mesh/RockBlue"));
	if (MeshObj.Succeeded())
		RockMesh->SetStaticMesh(MeshObj.Object);
}

// Called when the game starts or when spawned
void ARockBlue::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARockBlue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


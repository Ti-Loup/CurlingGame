// Fill out your copyright notice in the Description page of Project Settings.


#include "BroomBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABroomBase::ABroomBase()
{
 	// Set this actor to call Tick() every frame.  
	PrimaryActorTick.bCanEverTick = false;

	/*
	//Creation de la mesh
	BroomMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BroomMesh"));
	RootComponent = BroomMesh;

	//Simuler physics
	BroomMesh->SetSimulatePhysics(true);
	BroomMesh->SetCollisionProfileName(TEXT("PhysicsActor"));
	*/

	//Box de collision autour du balet
	InteractionCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionZoneCollision"));
	InteractionCollision->SetupAttachment(PickUpMesh);
	//Taille de la collision
	InteractionCollision->SetBoxExtent(FVector(20.f, 100.f, 20.f));//vector de type float
	InteractionCollision->SetCollisionProfileName(TEXT("Trigger"));
}

// Called when the game starts or when spawned
void ABroomBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABroomBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "FightingStone.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
// Sets default values
AFightingStone::AFightingStone()
{
 	
	PrimaryActorTick.bCanEverTick = false;
	/*
	* 
	* Deja fais dans PickUpObject -> Polymorphisme
	//creation mesh
	FightingStoneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FightStoneMesh"));
	RootComponent = FightingStoneMesh;

	//Simuler physics
	FightingStoneMesh->SetSimulatePhysics(true);
	FightingStoneMesh->SetCollisionProfileName(TEXT("PhysicsActor"));
	*/

	//Box de collision autour du balet
	InteractionCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionZoneCollision"));
	InteractionCollision->SetupAttachment(PickUpMesh);
	//Taille de la collision
	InteractionCollision->SetBoxExtent(FVector(20.f, 20.f, 20.f));//vector de type float
	InteractionCollision->SetCollisionProfileName(TEXT("Trigger"));
}

// Called when the game starts or when spawned
void AFightingStone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFightingStone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FightingStone.generated.h"

UCLASS()
class CURLING_API AFightingStone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFightingStone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//mesh de la pierre de combat
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FightStone")
	class UStaticMeshComponent* FightingStoneMesh;
	//BoxCollision pour interagir avec le joueur
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FightStone")
	class UBoxComponent* InteractionCollision;

};

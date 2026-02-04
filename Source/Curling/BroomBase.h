// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BroomBase.generated.h"

UCLASS()
class CURLING_API ABroomBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABroomBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	//creation mesh broom
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Broom")
	class UStaticMeshComponent* BroomMesh;

	//Zone Collision
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Broom")
	class UBoxComponent* InteractionCollision;
};

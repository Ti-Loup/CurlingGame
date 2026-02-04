// Fill out your copyright notice in the Description page of Project Settings.

/*
distances des pierres au cercle
ordre 
points du bout

pointeur contient une adresse et non valeur.
*/

#include "ZoneMaison.h"
#include "RockBase.h"
#include "MatchGameMode.h"
#include "Components/SphereComponent.h"


// Sets default values
AZoneMaison::AZoneMaison()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//constructeur pour declarer les pierres dans la maison
	ZoneMaison = CreateDefaultSubobject<USphereComponent>(TEXT("ZoneMaison"));
	RootComponent = ZoneMaison;

	ZoneMaison->SetSphereRadius(180.f);//taille de la maison (cercle)
	ZoneMaison->SetCollisionProfileName(TEXT("Trigger"));

}

//Combien de points équipe bleu ou rouge gagnent
void AZoneMaison::CalculerScore(int32& OutBluePoints, int32& OutRedPoints) {
	//Scores À 0 avant de calculer.
	OutBluePoints = 0;
	OutRedPoints = 0;
	//Tableau array pour récuperer les pierres qui touche la sphere de la ZoneMaison
	TArray<AActor*> OverlappingActors;
	ZoneMaison->GetOverlappingActors(OverlappingActors, ARockBase::StaticClass());


	//structure stocker rock + la distance (public) 
	struct FRockDistance
	{
		ARockBase* Rock;
		float Distance;
	};

	TArray<FRockDistance> Rocks;

	const FVector Center = GetActorLocation();
	//Le centre de la maison
	
	//pour chaque pierre qui touche, calcule de la distance au centre 
	for (AActor* Actor : OverlappingActors)
	{
		ARockBase* Rock = Cast<ARockBase>(Actor);
		if (!Rock) continue;//si pas une pierre -> continue

		float Dist = FVector::Dist(Rock->GetActorLocation(), Center);
		Rocks.Add({ Rock, Dist });//stocker 
	}

	
	if (Rocks.Num() == 0) return; //Pierre dans maison == 0 alors pas de score

	//Tri des pierres par distance : Plus près de 0 en premier  <  
	Rocks.Sort([](const FRockDistance& A, const FRockDistance& B)
		{
			return A.Distance < B.Distance;
		});

	//L’équipe de la pierre la plus proche est la seule à pouvoir marquer des points.
	ETeam ScoringTeam = Rocks[0].Rock->GetTeam();

	
	for (const FRockDistance& Entry : Rocks)
	{
		if (Entry.Rock->GetTeam() == ScoringTeam)
		{
			(ScoringTeam == ETeam::Blue) ? OutBluePoints++ : OutRedPoints++;
		}
		else
		{
			break; // pierre adverse coupe les points
		}
	}

}
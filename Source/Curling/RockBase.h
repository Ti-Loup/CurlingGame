

#pragma once

#include "CoreMinimal.h"
#include "MatchGameMode.h"//pour ETeam
#include "GameFramework/Actor.h"
#include "RockBase.generated.h"

UCLASS()
class CURLING_API ARockBase : public AActor
{
    GENERATED_BODY()

public:
    ARockBase();

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

public:

    // Nombre de tours choisi par le joueur
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int RotationVelocity = 0;

    // Valeur de friction calculée selon rotation
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float FrictionValue = 0.0f;
    float GetFriction();

    // Verifier que la pierre c'est bien arrêtée
    bool RockHasStopped = false;
    float TimeBelowSpeed = 0.f;

    // Facteur qui multiplie la force du virage
    float CurlRotationFactor = 1.0f;
    //                                                                               ---     Méthodes     ---
   
    
    void NoCurl();
    
    //Debut et fin de la Force linéaire du lancer du joueur 
    void StartCharging();
    void StopCharging();
    //fonction du lancer de pierre
    void LaunchRock();
    //notification lorsque la pierre s'arrete pour MatchGameMode.
    void NotifyStoneStopped();


    //Force linéaire du lancer par le joueur
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentForce = 0.0f;
    //Force Max du lancer
    UPROPERTY(EditAnywhere, BlueprintReadWrite)

    //Force Maximal du lancer
    float MaxForce = 2000.0f;
    //La vitesse de chargement du lancer de 0 à 2000f
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ForceChargeRate = 400.0f; // vitesse de la barre
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool ForceIsCharging = false;
 
    
    //mesh de la roche
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* RockMesh;

//Joueur vise gauche ou droite avant de lancer pour la trajectoire
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AimAngle = 0.0f; 


    //GetTeam() est utiliser aussi dans ZoneMaison
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    ETeam Team;
    ETeam GetTeam() const { return Team; }



    // La vitesse manuelle de la pierre
    float ManualSpeed = 0.0f;

    // Variable pour savoir si la pierre glisse
    bool bIsSliding = false;


    void UpdateFrictionFromRotation();
    //Pour la force de la curl
    float curlSide;

private:
    //Ici les variables sont pour que la pierre ne commence pas a la vitesse maximal de base ,mais elle est progressive vers le peak de la vitesse.
    bool bisBeingPushed = false;
    float PushTimer = 0.0f;
    float MaxPushDuration = 3.75f;
    FVector pushDirection;

   
};

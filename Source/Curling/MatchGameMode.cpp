// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchGameMode.h"
#include "RockBase.h"
#include "ZoneMaison.h"
#include "CurlingPlayerController.h"
#include "Kismet/GameplayStatics.h"//Pour le gameplay static
#include "Engine/Engine.h"


//Regle pour savoir qui commence avec marteau

AMatchGameMode::AMatchGameMode()
{
    PlayerControllerClass = ACurlingPlayerController::StaticClass();
}



//Begin Play
void AMatchGameMode::BeginPlay()
{
    Super::BeginPlay();
    MatchPhase = EMatchPhase::Warmup;//Mo dew libre au depart
    
    // Cherche l'acteur ZoneMaison qui est placé dans ta carte
    AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AZoneMaison::StaticClass());

    if (FoundActor)
    {
        // objet trouver assigner a la variable
        ZoneMaison = Cast<AZoneMaison>(FoundActor);

        if (ZoneMaison)//Verifier que la zonemaison est bien connecte avec un debugmessage
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Zone Maison connectée au GameMode"));
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ERREUR Aucune ZoneMaison"));
    }

    //Pour qui a le marteau 
    HammerTeam = FlipCoin();

    //message pour dire qui commence 
    if (HammerTeam == ETeam::Blue)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("L'equipe Bleue a le marteau !"));
    }
    else{
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("L'equipe Rouge a le marteau !"));
    };

    //fonction StartEnd pour savoir qui commence à lancer
    StartEnd();
}



//Fonction FlipCoin() called au beginplay
ETeam AMatchGameMode::FlipCoin()
{
    return (FMath::RandRange(0, 1) == 0) ? ETeam::Blue : ETeam::Red;
}

//choix entre Match cour de 4 bouts ou Match complet de 10 bouts.
int32 AMatchGameMode::GetNumberOfEnds() const
{
    switch (MatchLength)
    {
    case EMatchLength::Short:
        return 4;

    case EMatchLength::Long:
        return 10;

    default://default long
        return 10;
    }
}

//Pour savoir qui commence à lancer
void AMatchGameMode::StartEnd() {

    StonesPlayedThisEnd = 0;
    MatchPhase = EMatchPhase::Playing;

    //L'equipe qui n'a pas le marteau commence
    CurrentTeam = (HammerTeam == ETeam::Blue)
        ? ETeam::Red
        : ETeam::Blue;
}

//apres qu'une pierre est lancé 
void AMatchGameMode::OnStonePlayed()
{
    StonesPlayedThisEnd++; //0 de base 16 max

    // Changer d'équipe après chaque pierre
    CurrentTeam = (CurrentTeam == ETeam::Blue) ? ETeam::Red : ETeam::Blue;

    // 16 pierres lancées = fin du bout
    if (StonesPlayedThisEnd >= StonesPerTeam * 2)
    {
        MatchPhase = EMatchPhase::BetweenEnds;
        HandleBetweenEnds();//fonction appeler
    }
}





//Fin d'un bout. Modification des points et le marteau
void AMatchGameMode::FinDuBout(int32 BluePoints, int32 RedPoints)
{
    // Ajouter les points
    BlueScore += BluePoints;
    RedScore += RedPoints;

   
    if (BluePoints > 0)
    {
        // Bleu a marqué alors Rouge a le marteau
        HammerTeam = ETeam::Red;
    }
    else if (RedPoints > 0)
    {
        // Rouge a marqué alors Bleu a le marteau
        HammerTeam = ETeam::Blue;
    }
    

    //conserve le marteau si aucun point
    CurrentTeam = HammerTeam;
}

//fin du bout et commencement de la prochaine
void AMatchGameMode::HandleBetweenEnds()
{
    MatchPhase = EMatchPhase::BetweenEnds;

    GEngine->AddOnScreenDebugMessage(
        -1, 4.f, FColor::White,
        TEXT("Fin du bout")
    );

   
    int32 BluePoints = 0;
    int32 RedPoints = 0;

    if (ZoneMaison) {
        ZoneMaison->CalculerScore(BluePoints, RedPoints);
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Erreur ZoneMaison non assigné | MatchGameMode"));
    }



    FinDuBout(BluePoints, RedPoints);

    CurrentEnd++;

    if (CurrentEnd >= GetNumberOfEnds())//si le nombre de manche est >= au nombre de match maximum choisie (4 cour ou 10 long)
    {
        MatchFinished();
    }
    else
    {
        StartEnd(); // démarre le prochain bout et on retourne au MatchPhase
    }
}




//Partie terminé

void AMatchGameMode::MatchFinished()
{
    MatchPhase = EMatchPhase::Ended;

    FString Winner =
        (BlueScore > RedScore) ? TEXT("BLEU GAGNE !") :
        (RedScore > BlueScore) ? TEXT("ROUGE GAGNE !") :
        TEXT("ÉGALITÉ !");

    GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Yellow, Winner);
}




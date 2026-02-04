// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZoneMaison.h"
#include "GameFramework/GameModeBase.h"
#include "MatchGameMode.generated.h"


class AZoneMaison;

UENUM(BlueprintType)
enum class EMatchPhase : uint8
{
	Warmup,      // libre
	Playing,     // tour de chacun de jouer
	BetweenEnds, // fin de manche/libre dans la map
	Ended        // match fini

    /* Warmup -> 0
       Playing -> 1
       BetweenEnds ->2
       Ended -> 3
    */
};

//enumeration pour montrer les equipes bleu et rouge

UENUM(BlueprintType)
enum class ETeam : uint8
{
	Blue,
	Red

    //Blue -> 0
    //Red -> 1
};
//Enumeration pour choisir entre match de 4 bouts ou 10

UENUM(BlueprintType)
enum class EMatchLength : uint8
{
	Short UMETA(DisplayName = "4 Bouts"),
	Long  UMETA(DisplayName = "10 Bouts")

    //Short ->0
    //Long ->1
};

UCLASS()
class CURLING_API AMatchGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
    AMatchGameMode();

    virtual void BeginPlay() override;



    // Match configuration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Match")
    EMatchLength MatchLength = EMatchLength::Long;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    EMatchPhase MatchPhase = EMatchPhase::Warmup;




    // State
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    ETeam CurrentTeam;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 CurrentEnd = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 StonesPlayedThisEnd = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int32 StonesPerTeam = 8;




    // Scores
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 BlueScore = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 RedScore = 0;

    // Pour la classe ZoneMaison
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Curling")
    AZoneMaison* ZoneMaison;


    // Logique et methode
    int32 GetNumberOfEnds() const;
    void StartEnd();
    void FinDuBout(int32 BluePoints, int32 RedPoints);

    void OnStonePlayed();//RockBase NotifyStoneStopped()

    

protected:
    ETeam HammerTeam;

    ETeam FlipCoin();
    void MatchFinished();
    void HandleBetweenEnds();

};

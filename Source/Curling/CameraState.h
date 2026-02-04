// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ECameraState : uint8 {
	//les etats de la camera des joueurs
	FollowPlayer UMETA(DisplayName = "Follow Player"),
	FollowStone  UMETA(DisplayName = "Follow Stone")//suit la pierre quand le joueur la lance

};

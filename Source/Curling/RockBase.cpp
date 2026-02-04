

#include "RockBase.h"
#include "MatchGameMode.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ARockBase::ARockBase()
{
 	
    PrimaryActorTick.bCanEverTick = true;
    //creation de la mesh 
    RockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RockMesh"));
    RootComponent = RockMesh;

    RockMesh->SetSimulatePhysics(true);
    RockMesh->SetEnableGravity(true);
}

// Called when the game starts or when spawned
void ARockBase::BeginPlay()
{
	Super::BeginPlay();
	
    RockMesh->SetLinearDamping(0.0f);//0f car on veut gerer le Linear Damping dans le tick manuellement
    RockMesh->SetAngularDamping(1.0f);
	


   
}

void ARockBase::Tick(float DeltaTime)
{
   
    Super::Tick(DeltaTime);

    // 
    // La phase de chargement
    if (ForceIsCharging)
    {
        CurrentForce += ForceChargeRate * DeltaTime;
        CurrentForce = FMath::Clamp(CurrentForce, 0.0f, MaxForce);
        GEngine->AddOnScreenDebugMessage(1, 0.1f, FColor::Cyan, FString::Printf(TEXT("Force : %f"), CurrentForce));
        return;
    }

    // Sécurité
    if (!bIsSliding || !RockMesh) return;


   
    // detection de collision entre les pierres pour les takeouts
    float CurrentPhysicsSpeed = RockMesh->GetPhysicsLinearVelocity().Size();

    // Si il y a un choc entre 2 pierres , on force la vitesse manuelle à s'adapter
    if (CurrentPhysicsSpeed < (ManualSpeed - 20.0f))
    {
        ManualSpeed = CurrentPhysicsSpeed;
    }

    // Calcul de la friction

    // Base de freinage La glace elle meme
    float BaseDeceleration = 42.0f;

    // avec le switch du nombre de tours choisit
    // Si Rotation 4 (Friction 0.10) -> 0.10 * 15 = +1.5 -> Total 41.5 peu de frein
    // Si Rotation 0 (Friction 1.00) -> 1.00 * 15 = +15  -> Total 55.0 ford frein
    float SwitchImpact = FrictionValue * 15.0f;

    // freinage de base de la glace + la friction de la pierre
    float TotalDeceleration = BaseDeceleration + SwitchImpact;

    ManualSpeed -= TotalDeceleration * DeltaTime;

    // Gestion de l'arret de la pierre 
    if (ManualSpeed <= 0.0f)
    {
        ManualSpeed = 0.0f;
        bIsSliding = false;
        RockMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);

        if (!RockHasStopped) {
            RockHasStopped = true;
            NotifyStoneStopped();
        }
        return;
    }



    // Appliquer le movement de la pierre
    FVector CurrentVelocity = RockMesh->GetPhysicsLinearVelocity();
    FVector Direction = CurrentVelocity.GetSafeNormal();

    // Fix direction départ
    if (Direction.IsNearlyZero()) Direction = GetActorForwardVector();

    // Logique du Curl (Virage)
    if (RotationVelocity >= 0) // On inclut 0 maintenant pour curl fort
    {
        FVector CurlDir = FVector::CrossProduct(Direction, FVector::UpVector);

        // valeur de base du curl
        float BaseTurnIntensity = 0.00005f;

        // Base * Le Sens (+1/-1) * Le Facteur du Switch
        // Si Rotation 4 ->  curl petit
        // Si Rotation 0 ->  gros curl
        FVector FinalTurn = CurlDir * BaseTurnIntensity * curlSide * CurlRotationFactor;

        Direction = (Direction + FinalTurn).GetSafeNormal();
    }

    // Commande finale au moteur physique
    FVector FinalVelocity = Direction * ManualSpeed;
    FinalVelocity.Z = 0;

    RockMesh->SetPhysicsLinearVelocity(FinalVelocity);
}

float ARockBase::GetFriction()
{
    return FrictionValue;
}

//Methode Si le joueur décide de pas mettre de curl sur la pierre(0)
void ARockBase::NoCurl()
{
    if (RotationVelocity == 0)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,
            TEXT("Lancé non conforme, À l'autre équipe de jouer "));//texte affiché si il n'y a pas de Rotation sur la pierre
    }
}


void ARockBase::StartCharging() {
    CurrentForce = 0.f;
    ForceIsCharging = true;
}

void ARockBase::StopCharging() {
    // Empêche dépasser la limite
    CurrentForce = FMath::Clamp(CurrentForce, 0.0f, MaxForce);//maxforce = 2000f

    // Utilise la force pour le lancer
    UE_LOG(LogTemp, Warning, TEXT("Pierre lancée avec force : %f"), CurrentForce);

    ForceIsCharging = false;

    LaunchRock();
}

void ARockBase::LaunchRock() {

    if (!RockMesh) return;

    UpdateFrictionFromRotation();//appel de la fonction Pour la modification de la friction de la pierre
    RockMesh->WakeRigidBody();
    RockMesh->SetLinearDamping(0.0f);
    RockMesh->SetAngularDamping(0.0f);

    float Ratio = FMath::Clamp(CurrentForce / MaxForce, 0.0f, 1.0f);

    // Modification de la puissance pour takeout
    ManualSpeed = Ratio * 950.0f;

    bIsSliding = true;

    FVector Forward = GetActorForwardVector();
    RockMesh->SetPhysicsLinearVelocity(Forward * ManualSpeed);

    UE_LOG(LogTemp, Warning, TEXT("Lancer ! Vitesse départ : %f"), ManualSpeed);
}
// Calculer la friction de la pierre selon le nombre de tours
void ARockBase::UpdateFrictionFromRotation()
{
    // Rotation 4 = Le joueur veut que ça glisse (Friction %)
    // Rotation 0 = Le joueur veut que ça freine (Friction 100%)

    switch (RotationVelocity)
    {
    case 4: // Rotation MAX
        FrictionValue = 0.10f;       // Glisse très loin
        CurlRotationFactor = 0.5f;   // Trajectoire très DROITE (Stable)
        break;

    case 3:
        FrictionValue = 0.30f;
        CurlRotationFactor = 0.8f;
        break;

    case 2: // Standard
        FrictionValue = 0.50f;
        CurlRotationFactor = 1.0f;   // Normal
        break;

    case 1:
        FrictionValue = 0.75f;
        CurlRotationFactor = 1.5f;
        break;

    case 0: // Rotation NULLE (Knuckleball)
        FrictionValue = 1.0f;        // Freine fort
        CurlRotationFactor = 0.0f;   // CURL ÉNORME (Incontrôlable/Virage sec)
        break;

    default:
        FrictionValue = 0.5f;
        CurlRotationFactor = 1.0f;
        break;
    }
}

void ARockBase::NotifyStoneStopped() {
    AMatchGameMode* GameMode = Cast<AMatchGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

    if (GameMode) {
        GameMode->OnStonePlayed();
    }
}
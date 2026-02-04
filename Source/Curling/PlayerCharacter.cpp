#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include"InputMappingContext.h"
#include "EnhancedInputSubsystems.h" 
#include "RockBase.h"
#include "BroomBase.h"
#include "TimerManager.h"//Pour appuyer sur e et le relacher
#include "CurlingPlayerController.h"
#include "GameFramework/SpringArmComponent.h"//Pour La camera SpringArm
#include "GameFramework/CharacterMovementComponent.h"//Pour camera 3eme personne


APlayerCharacter::APlayerCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	//ajout du SpringArm 
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;


	//creation de la camera
	Camera = CreateDefaultSubobject<UCameraComponent>("Player Camera");
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;//ajouter pour 3eme et 1eme
	Camera->SetRelativeLocation(FVector(0.f, 0.f, 60.f));//ajouter pour  3eme persoone camera angle de vue


	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;//Pour 3eme personne


}


void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//add input mapping context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{	//get local player subsystem
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			//add input context
			Subsystem->AddMappingContext(IMC_Player, 0);
		}
		
	}

}


void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &APlayerCharacter::Jump);

		Input->BindAction(IA_Movement, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		Input->BindAction(IA_Camera, ETriggerEvent::Triggered, this, &APlayerCharacter::CameraLook);

		Input->BindAction(IA_ToggleView, ETriggerEvent::Triggered, this, &APlayerCharacter::ToggleView); 

		//Inputs pour les emotes
		Input->BindAction(IA_Emote1, ETriggerEvent::Started, this, &APlayerCharacter::EmoteUn);//Ajouter pour faire jouer Emote1 (1)
		Input->BindAction(IA_Emote1, ETriggerEvent::Completed, this, &APlayerCharacter::StopEmote);//Ajouter pour faire jouer Emote1
		Input->BindAction(IA_Emote2, ETriggerEvent::Started, this, &APlayerCharacter::EmoteDeux);//Ajouter pour faire jouer Emote2
		Input->BindAction(IA_Emote2, ETriggerEvent::Completed, this, &APlayerCharacter::StopEmote);//Ajouter pour faire jouer Emote2
		Input->BindAction(IA_Emote3, ETriggerEvent::Started, this, &APlayerCharacter::EmoteTrois);//Ajouter pour faire jouer Emote3
		Input->BindAction(IA_Emote3, ETriggerEvent::Completed, this, &APlayerCharacter::StopEmote);//Ajouter pour faire jouer Emote3
		Input->BindAction(IA_Emote4, ETriggerEvent::Started, this, &APlayerCharacter::EmoteQuatre);//Ajouter pour faire jouer Emote4
		Input->BindAction(IA_Emote4, ETriggerEvent::Completed, this, &APlayerCharacter::StopEmote);//Ajouter pour faire jouer Emote4

		//Input pour lancer la pierre (F)
		Input->BindAction(IA_RockThrow, ETriggerEvent::Started, this, &APlayerCharacter::InteractCharge);
		Input->BindAction(IA_RockThrow, ETriggerEvent::Completed, this, &APlayerCharacter::InteractStopCharge);
		
		//Input pour choisir le nombre de rotation de la pierre avec le switch
		Input->BindAction(IA_AdjustRotation, ETriggerEvent::Triggered, this, &APlayerCharacter::AdjustRotation);
		
		// Nouveau binding pour le Curl
		Input->BindAction(IA_ToggleCurl, ETriggerEvent::Triggered, this, &APlayerCharacter::ToggleCurlSide);

		//Ajout Input Pour Interagir avec Broom (E)
		Input->BindAction(IA_Interaction, ETriggerEvent::Started, this, &APlayerCharacter::QuandInteractionAppuyer);
		Input->BindAction(IA_Interaction, ETriggerEvent::Completed, this, &APlayerCharacter::QuandInteractionRelacher);
		Input->BindAction(IA_Interaction, ETriggerEvent::Canceled, this, &APlayerCharacter::QuandInteractionRelacher);

	}
	
}

//Methode Move()
void APlayerCharacter::Move(const FInputActionValue& InputValue) {
	FVector2D InputVector = InputValue.Get<FVector2D>();

	//si le controller est valid alors on peut bouger
	if (IsValid(Controller)) {
	//Get Forward Direction			Roll / Pitch / Yaw
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		//Add movement input
		AddMovementInput(ForwardDirection, InputVector.Y); //Pressing w +1 / Pressing s -1
		AddMovementInput(RightDirection, InputVector.X);
	}
}



//Methode CameraLook()
void APlayerCharacter::CameraLook(const FInputActionValue& InputValue) {
	
	FVector2D InputVector = InputValue.Get<FVector2D>();

	if (IsValid(Controller)) {
		AddControllerYawInput(InputVector.X);
		AddControllerPitchInput(InputVector.Y);
	}

}



//Methode Jump()
void APlayerCharacter::Jump() {
	ACharacter::Jump();
}
//Methode pour changer entre 1eme et 3eme 
void APlayerCharacter::ToggleView()
{
	switch (CameraView)
	{
	case ECameraView::FirstPerson:
		CameraView = ECameraView::ThirdPersonClose;
		break;

	case ECameraView::ThirdPersonClose:
		CameraView = ECameraView::ThirdPersonFar;
		break;

	case ECameraView::ThirdPersonFar:
		CameraView = ECameraView::FirstPerson;
		break;
	}

	ApplyCameraView();
}

//BOOL SI ON VEUT PASSER DE 1ERE A 3EME PERSON	
//Switch Si D'AVANTAGE 
void APlayerCharacter::ApplyCameraView()
{
	switch (CameraView)
	{
	case ECameraView::FirstPerson:
		SpringArm->TargetArmLength = FirstPersonArmLength;
		SpringArm->bDoCollisionTest = false;
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		break;

	case ECameraView::ThirdPersonClose:
		SpringArm->TargetArmLength = ThirdPersonArmLengthONE;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		break;

	case ECameraView::ThirdPersonFar:
		SpringArm->TargetArmLength = ThirdPersonArmLengthTWO;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		break;
	}
}
//Lorsque l'emote commence 
void APlayerCharacter::EmoteUn() {
	
	bWantsToEmote1 = true;

}
void APlayerCharacter::EmoteDeux() {

	bWantsToEmote2 = true;
}
void APlayerCharacter::EmoteTrois() {

	bWantsToEmote3 = true;
}
void APlayerCharacter::EmoteQuatre() {

	bWantsToEmote4 = true;
}

//Lorsque l'emote se termine ou pour arreter l'emote en cour
void APlayerCharacter::StopEmote() {
	bWantsToEmote1 = false;
	bWantsToEmote2 = false;
	bWantsToEmote3 = false;
	bWantsToEmote4 = false;
}
//fonction lorsque le joueur appuis la force commence a monter
void APlayerCharacter::InteractCharge() {

	//GEngine->AddOnScreenDebugMessage(-1,5.0f, FColor::Red, TEXT("SA FONCTIONNE ?"));
	//si il y a une pierre actuelle dans le niveau alors il commence a charger
	if (CurrentRock) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("SA FONCTIONNE "));
		CurrentRock->StartCharging();
	 }
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("nop ?"));
	}
	
	
}
//Lorsque le joueur arrete d'apuier alors cela s'arrete et pierre lancer
void APlayerCharacter::InteractStopCharge() {
	if (CurrentRock) {
		CurrentRock->StopCharging();

		CurrentRock = nullptr;

		if (ACurlingPlayerController* PC = Cast<ACurlingPlayerController>(GetController())) {
			PC->SetCameraFollowStone(PC->CurrentStone);
		}
	}
}
//Pour ajuster la rotation en fonction de mon switch dans RockBase
void APlayerCharacter::AdjustRotation(const FInputActionValue& Value)
{
	// On récupère la valeur (1.0 = Haut, -1.0 = Bas)
	float Direction = Value.Get<float>();

	if (CurrentRock && !CurrentRock->bIsSliding)
	{
		// --- MOLETTE VERS LE HAUT (Direction > 0) ---
		if (Direction > 0 && CurrentRock->RotationVelocity < 4)
		{
			CurrentRock->RotationVelocity++;
			GEngine->AddOnScreenDebugMessage(55, 1.f, FColor::Green,
				FString::Printf(TEXT("Rotation : %d (Glisse +)"), CurrentRock->RotationVelocity));
		}
		// --- MOLETTE VERS LE BAS (Direction < 0) ---
		else if (Direction < 0 && CurrentRock->RotationVelocity > 0)
		{
			CurrentRock->RotationVelocity--;
			GEngine->AddOnScreenDebugMessage(55, 1.f, FColor::Red,
				FString::Printf(TEXT("Rotation : %d (Glisse -)"), CurrentRock->RotationVelocity));
		}
	}
}
//cette fonction sert a savoir la curl s'il doit etre vers la gauche ou droite a l'aide du joueur choix
void APlayerCharacter::ToggleCurlSide(const FInputActionValue& Value)
{
	// On récupère la valeur (-1 pour Gauche, +1 pour Droite)
	float InputDirection = Value.Get<float>();

	// On vérifie qu'on tient une pierre et qu'elle n'est pas partie
	if (CurrentRock && !CurrentRock->bIsSliding)
	{
		// Si on appuie sur DROITE (Valeur positive)
		if (InputDirection > 0)
		{
			CurrentRock->curlSide = -1.0f; // 1 = Droite
			GEngine->AddOnScreenDebugMessage(60, 0.1f, FColor::Yellow, TEXT("Curl vers la droite ---> (Sens Horaire)"));
		}
		// Si on appuie sur GAUCHE (Valeur négative)
		else if (InputDirection < 0)
		{
			CurrentRock->curlSide = 1.0f; // -1 = Gauche
			GEngine->AddOnScreenDebugMessage(60, 0.1f, FColor::Yellow, TEXT("Curl cers la gauche <--- (Sens Anti-Horaire) "));
		}
	}
}

void APlayerCharacter::QuandInteractionAppuyer() {
	TestInput();
	// Le personnage ne tiens rien -> ramasse de suite
	if (!HeldBroom)
	{
		GrabObject(); // appelm fonction pour prendre l'object
	}
	//le perso tiens déjà le balai -> lancement du chrono pour le lacher
	else
	{
		// On lance le timer de 1 seconde.
		// Si le temps s'écoule, il lancera "DropObject".
		GetWorldTimerManager().SetTimer(TimerHandle_DropInteraction, this, &APlayerCharacter::DropObject, 1.0f, false);

		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, TEXT("Maintiens pour lâcher"));
	}
}


void APlayerCharacter::QuandInteractionRelacher() {
	// Si je relâche le bouton avant le drop alors annulation
	GetWorldTimerManager().ClearTimer(TimerHandle_DropInteraction);
}


//Fonction pour grab un objet
void APlayerCharacter::GrabObject() {
	// Si on tient déjà quelque chose, on ne fait rien 
	if (HeldBroom) return;

	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, ABroomBase::StaticClass());

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("J'ai trouvé un balai"));
	
	for (AActor* Actor : OverlappingActors)
	{
		ABroomBase* RamasserLeBalais = Cast<ABroomBase>(Actor);
		if (RamasserLeBalais)
		{
			//Couper la physique
			RamasserLeBalais->BroomMesh->SetSimulatePhysics(false);
			RamasserLeBalais->BroomMesh->SetCollisionProfileName(TEXT("NoCollision"));

			//Attacher
			RamasserLeBalais->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Socket_Broom"));

			//Sauvegarder
			HeldBroom = RamasserLeBalais;

			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Je tiens le balai"));
			break;
		}
	}


}

void APlayerCharacter::DropObject()
{
	// on peut drop un objerct seulement si on a un object dans la main
	if (HeldBroom)//(HeldBroom = RammaserLeBalais)
	{
		//Détacher
		HeldBroom->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		//Réactiver la Physique
		HeldBroom->BroomMesh->SetSimulatePhysics(true);

		HeldBroom->BroomMesh->SetCollisionProfileName(TEXT("PhysicsActor"));
		HeldBroom->BroomMesh->WakeAllRigidBodies();
		HeldBroom->BroomMesh->AddImpulse(GetActorForwardVector() * 50.f, NAME_None, true);

		
		HeldBroom = nullptr;
	}
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT(" Object a ete drop "));
}



//Utiliser pour verifier que les Inputs fonctionnaient bien et que ce n'etait pas un bug

void APlayerCharacter::TestInput() {

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Pressed Action"));

}

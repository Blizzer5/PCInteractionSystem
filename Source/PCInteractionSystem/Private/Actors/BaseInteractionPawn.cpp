// Fill out your copyright notice in the Description page of Project Settings.
#include <Actors/BaseInteractionPawn.h>

// Sets default values
ABaseInteractionPawn::ABaseInteractionPawn()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

}

void ABaseInteractionPawn::OnInteractionStarted_Implementation(ACharacter* InteractedBy)
{
    BeingInteractedBy = InteractedBy;
    switch (InteractionType)
    {
    case EInteractionType::Straight:
    {
        Execute_OnInteractionCompleted(this);
    }
    break;
    case EInteractionType::Timer:
    {
        GetWorld()->GetTimerManager().SetTimer(InteractionTimerHandle, this, &ABaseInteractionPawn::CompleteInteraction, InteractionTime, false);
    }
    break;
    default:
        break;
    }
}

void ABaseInteractionPawn::StopInteraction_Implementation()
{
    GetWorld()->GetTimerManager().ClearTimer(InteractionTimerHandle);
}

AActor* ABaseInteractionPawn::GetActor_Implementation()
{
    return this;
}

bool ABaseInteractionPawn::IsInteractable_Implementation()
{
    return bIsInteractable;
}

void ABaseInteractionPawn::DisableInteraction_Implementation()
{
    bIsInteractable = false;
}

void ABaseInteractionPawn::EnableInteraction_Implementation()
{
    bIsInteractable = true;
}

void ABaseInteractionPawn::OnHighlighted_Implementation()
{
    for (UMeshComponent* mesh : GetMeshComponents())
    {
        mesh->SetRenderCustomDepth(true);
    }
}

void ABaseInteractionPawn::OnHighlightRemoved_Implementation()
{
    for (UMeshComponent* mesh : GetMeshComponents())
    {
        mesh->SetRenderCustomDepth(false);
    }
}

EInteractionType ABaseInteractionPawn::GetInteractionType_Implementation()
{
    return InteractionType;
}

FText ABaseInteractionPawn::GetInteractableName_Implementation()
{
    return InteractableName;
}

FText ABaseInteractionPawn::GetInteractionActionName_Implementation()
{
    return InteractionActionName;
}

float ABaseInteractionPawn::GetInteractionCompletedPercentage_Implementation()
{
    return (InteractionTime - GetWorld()->GetTimerManager().GetTimerRemaining(InteractionTimerHandle)) / InteractionTime;
}

void ABaseInteractionPawn::SetInteractableName_Implementation(const FText& newInteractionName)
{
    InteractableName = newInteractionName;
}

void ABaseInteractionPawn::SetInteractionActionName_Implementation(const FText& newInteractionActionName)
{
    InteractionActionName = newInteractionActionName;
}

void ABaseInteractionPawn::SetInteractionType_Implementation(EInteractionType newInteractionType)
{
    InteractionType = newInteractionType;
}

void ABaseInteractionPawn::SetInteractionTime_Implementation(float newInteractionTime)
{
    InteractionTime = newInteractionTime;
}

// Called when the game starts or when spawned
void ABaseInteractionPawn::BeginPlay()
{
    Super::BeginPlay();

    Execute_SetInteractableName(this, PawnInteractableName);
    Execute_SetInteractionActionName(this, InteractionActionName);
    Execute_SetInteractionType(this, PawnInteractionType);
    Execute_SetInteractionTime(this, PawnInteractionTime);
}

void ABaseInteractionPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ABaseInteractionPawn::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ABaseInteractionPawn::MoveRight);

    // We have 2 versions of the rotation bindings to handle different kinds of devices differently
    // "turn" handles devices that provide an absolute delta, such as a mouse.
    // "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("TurnRate", this, &ABaseInteractionPawn::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("LookUpRate", this, &ABaseInteractionPawn::LookUpAtRate);
}

void ABaseInteractionPawn::CompleteInteraction()
{
    Execute_OnInteractionCompleted(this);
}


TArray<UMeshComponent*> ABaseInteractionPawn::GetMeshComponents()
{
    TArray<UMeshComponent*> meshes;
    GetComponents(meshes);
    return meshes;
}

void ABaseInteractionPawn::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    for (UMeshComponent* mesh : GetMeshComponents())
    {
        mesh->SetRenderCustomDepth(false);
    }
}

// MOVEMENT
void ABaseInteractionPawn::TurnAtRate(float Rate)
{
    // calculate delta for this frame from the rate information
    AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABaseInteractionPawn::LookUpAtRate(float Rate)
{
    // calculate delta for this frame from the rate information
    AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABaseInteractionPawn::MoveForward(float Value)
{
    if ((Controller != NULL) && (Value != 0.0f))
    {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void ABaseInteractionPawn::MoveRight(float Value)
{
    if ((Controller != NULL) && (Value != 0.0f))
    {
        // find out which way is right
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get right vector 
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        // add movement in that direction
        AddMovementInput(Direction, Value);
    }
}
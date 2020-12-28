// Fill out your copyright notice in the Description page of Project Settings.
#include <Actors/BaseInteractionActor.h>

// Sets default values
ABaseInteractionActor::ABaseInteractionActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

}

void ABaseInteractionActor::OnInteractionStarted_Implementation(ACharacter* InteractedBy)
{
    switch (InteractionType)
    {
    case EInteractionType::Straight:
    {
        Execute_OnInteractionCompleted(this);
    }
    break;
    case EInteractionType::Timer:
    {
        GetWorld()->GetTimerManager().SetTimer(InteractionTimerHandle, this, &ABaseInteractionActor::CompleteInteraction, InteractionTime, false);
    }
    break;
    default:
        break;
    }
}

void ABaseInteractionActor::StopInteraction_Implementation()
{
    GetWorld()->GetTimerManager().ClearTimer(InteractionTimerHandle);
}

AActor* ABaseInteractionActor::GetActor_Implementation()
{
    return this;
}

bool ABaseInteractionActor::IsInteractable_Implementation()
{
    return bIsInteractable;
}

void ABaseInteractionActor::DisableInteraction_Implementation()
{
    bIsInteractable = false;
}

void ABaseInteractionActor::EnableInteraction_Implementation()
{
    bIsInteractable = true;
}

void ABaseInteractionActor::OnHighlighted_Implementation()
{
    for (UMeshComponent* mesh : GetMeshComponents())
    {
        mesh->SetRenderCustomDepth(true);
    }
}

void ABaseInteractionActor::OnHighlightRemoved_Implementation()
{
    for (UMeshComponent* mesh : GetMeshComponents())
    {
        mesh->SetRenderCustomDepth(false);
    }
}

EInteractionType ABaseInteractionActor::GetInteractionType_Implementation()
{
    return InteractionType;
}

FText ABaseInteractionActor::GetInteractableName_Implementation()
{
    return InteractableName;
}

FText ABaseInteractionActor::GetInteractionActionName_Implementation()
{
    return InteractionActionName;
}

float ABaseInteractionActor::GetInteractionCompletedPercentage_Implementation()
{
    return (InteractionTime - GetWorld()->GetTimerManager().GetTimerRemaining(InteractionTimerHandle)) / InteractionTime;
}

void ABaseInteractionActor::SetInteractableName_Implementation(const FText& newInteractionName)
{
    InteractableName = newInteractionName;
}

void ABaseInteractionActor::SetInteractionActionName_Implementation(const FText& newInteractionActionName)
{
    InteractionActionName = newInteractionActionName;
}

void ABaseInteractionActor::SetInteractionType_Implementation(EInteractionType newInteractionType)
{
    InteractionType = newInteractionType;
}

void ABaseInteractionActor::SetInteractionTime_Implementation(float newInteractionTime)
{
    InteractionTime = newInteractionTime;
}

// Called when the game starts or when spawned
void ABaseInteractionActor::BeginPlay()
{
    Super::BeginPlay();

    Execute_SetInteractableName(this, ActorInteractableName);
    Execute_SetInteractionActionName(this, InteractionActionName);
    Execute_SetInteractionType(this, ActorInteractionType);
    Execute_SetInteractionTime(this, ActorInteractionTime);
}

void ABaseInteractionActor::CompleteInteraction()
{
    Execute_OnInteractionCompleted(this);

    for (UMeshComponent* mesh : GetMeshComponents())
    {
        mesh->SetRenderCustomDepth(false);
    }
}

TArray<UMeshComponent*> ABaseInteractionActor::GetMeshComponents()
{
    TArray<UMeshComponent*> meshes;
    GetComponents(meshes);
    return meshes;
}


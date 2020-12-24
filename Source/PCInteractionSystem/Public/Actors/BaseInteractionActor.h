// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableInterface.h"
#include "BaseInteractionActor.generated.h"

UCLASS()
class PCINTERACTIONSYSTEM_API ABaseInteractionActor : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseInteractionActor();

    virtual void OnInteractionStarted_Implementation(ACharacter* InteractedBy) override;
    virtual void OnInteractionCompleted_Implementation() override;
    virtual void StopInteraction_Implementation() override;
    virtual void DisableInteraction_Implementation() override;
    virtual void EnableInteraction_Implementation() override;

    virtual AActor* GetActor_Implementation() override;
    virtual bool IsInteractable_Implementation() override;
    virtual EInteractionType GetInteractionType_Implementation() override;
    virtual FText GetInteractableName_Implementation() override;
    virtual FText GetInteractionActionName_Implementation() override;
    virtual float GetInteractionCompletedPercentage_Implementation() override;

    virtual void SetInteractableName_Implementation(const FText& newInteractionName) override;
    virtual void SetInteractionActionName_Implementation(const FText& newInteractionActionName) override;
    virtual void SetInteractionType_Implementation(EInteractionType newInteractionType) override;
    virtual void SetInteractionTime_Implementation(float newInteractionTime) override;

    // PROPERTIES
    UPROPERTY(EditAnywhere, Category = "Base Interaction Actor")
    FText ActorInteractableName;
    UPROPERTY(EditAnywhere, Category = "Base Interaction Actor")
    FText InteractionActionName;
    UPROPERTY(EditAnywhere, Category = "Base Interaction Actor")
    EInteractionType ActorInteractionType;
	UPROPERTY(EditAnywhere, Category = "Base Interaction Actor")
	float ActorInteractionTime;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
    UFUNCTION()
    void CompleteInteraction();
    
    FTimerHandle InteractionTimerHandle;
};

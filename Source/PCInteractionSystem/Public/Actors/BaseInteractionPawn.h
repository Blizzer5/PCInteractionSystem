// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableInterface.h"
#include "BaseInteractionPawn.generated.h"

UCLASS()
class PCINTERACTIONSYSTEM_API ABaseInteractionPawn : public APawn, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseInteractionPawn();

    virtual void OnInteractionStarted_Implementation(ACharacter* InteractedBy) override;
    virtual void StopInteraction_Implementation() override;
    virtual void DisableInteraction_Implementation() override;
    virtual void EnableInteraction_Implementation() override;
    virtual void OnHighlighted_Implementation() override;
    virtual void OnHighlightRemoved_Implementation() override;

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
    UPROPERTY(EditAnywhere, Category = "Base Interaction Pawn")
    FText PawnInteractableName;
    UPROPERTY(EditAnywhere, Category = "Base Interaction Pawn")
    FText InteractionActionName;
    UPROPERTY(EditAnywhere, Category = "Base Interaction Pawn")
    EInteractionType PawnInteractionType;
	UPROPERTY(EditAnywhere, Category = "Base Interaction Pawn")
	float PawnInteractionTime;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(BlueprintReadWrite, Category = "Base Interaction Pawn")
        ACharacter* BeingInteractedBy = nullptr;

    /** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
        float BaseTurnRate;

    /** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
        float BaseLookUpRate;

    void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    void TurnAtRate(float Rate);
    void LookUpAtRate(float Rate);
    void MoveForward(float Value);
    void MoveRight(float Value);
   

private:
    UFUNCTION()
    void CompleteInteraction();

    TArray<UMeshComponent*> GetMeshComponents();
    
    FTimerHandle InteractionTimerHandle;
    
public:
    void PossessedBy(AController* NewController) override;

};

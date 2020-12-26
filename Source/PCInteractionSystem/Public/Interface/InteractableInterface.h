// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

UENUM(BlueprintType)
enum class EInteractionType : uint8
{
    None,
	// Interact with the object right way
    Straight,
	// Interact after some time
    Timer,
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PCINTERACTIONSYSTEM_API IInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable Interface")
	AActor* GetActor();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable Interface")
	void OnInteractionStarted(ACharacter* InteractedBy);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable Interface")
	void OnInteractionCompleted();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable Interface")
	void OnHighlighted();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable Interface")
	void OnHighlightRemoved();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable Interface")
	void StopInteraction();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable Interface")
	EInteractionType GetInteractionType();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable Interface")
	FText GetInteractableName();
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable Interface")
    FText GetInteractionActionName();
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable Interface")
    bool IsInteractable();
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable Interface")
    float GetInteractionCompletedPercentage();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable Interface")
    void DisableInteraction();
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable Interface")
    void EnableInteraction();
    
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable Interface")
    void SetInteractableName(const FText& newInteractionName);
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable Interface")
    void SetInteractionActionName(const FText& newInteractionActionName);
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable Interface")
    void SetInteractionType(EInteractionType newInteractionType);
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable Interface")
    void SetInteractionTime(float newInteractionTime);

	EInteractionType InteractionType;
	FText InteractableName;
	FText InteractionActionName;
	float InteractionTime;
	bool bIsInteractable = true;
};

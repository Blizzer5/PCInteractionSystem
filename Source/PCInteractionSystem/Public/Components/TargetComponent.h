// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Materials/Material.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/Actor.h"
#include "Delegates/DelegateCombinations.h"
#include "TargetComponent.generated.h"

class IInteractableInterface;

UENUM(BlueprintType)
enum class ECameraType : uint8{
    FirstPerson,
    ThirdPerson,
};

USTRUCT(Blueprintable)
struct FMaterialInfo {
    GENERATED_BODY()

    FMaterialInfo() = default;

    FMaterialInfo(UMaterial* materialPointer, FName slot) :
    material(materialPointer) ,
    materialSlot(slot)
    {
        
    }

    UPROPERTY()
    UMaterial* material;

    UPROPERTY()
    FName materialSlot;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHighlightedObjectChanged, TScriptInterface<IInteractableInterface>, IInteractableObject);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorHighlightedChanged, AActor*, currentHighlightedActor, AActor*, lastHighlightedActor, FMaterialInfo, materialInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionStarted, AActor*, InteractedActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionInterrupted, AActor*, InteractedActor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PCINTERACTIONSYSTEM_API UTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
    UTargetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

    UFUNCTION()
    void InitializeComponent();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AutoTarget | Third Person | Line of Sight", meta = (AllowPrivateAccess = "true", UIMin = "1.0", UIMax = "360.0"))
        float ArcAngle;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AutoTarget | Third Person | Line of Sight", meta = (AllowPrivateAccess = "true", UIMin = "1.0", UIMax = "5.0"))
        float AngleStep;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AutoTarget | Third Person | Line of Sight", meta = (AllowPrivateAccess = "true", UIMin = "200", UIMax = "1000"))
        float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AutoTarget | Camera", meta = (AllowPrivateAccess = "true"))
		ECameraType cameraType;

	UPROPERTY(EditAnywhere, Category = "AutoTarget | Debug")
		bool bOnlyInteractableObjects = true;

    UPROPERTY(EditAnywhere, Category = "AutoTarget | Debug")
        bool bShowDebug;

public:
    UFUNCTION(BlueprintPure, Category = "Auto Target")
    AActor* GetLastHighlightedObject();
    UFUNCTION()
    void Interact();
    UFUNCTION()
    void InteractReleased();

    void DisableTargeting();
    void EnableTargeting();

    UFUNCTION()
    void OnControllerOwnerChangedPawn();
private:
    void GatherObjects(float DeltaSeconds);

    FMaterialInfo GetHitMaterialInfo(FHitResult &HitResult);

    void HighlightClosestObject();
    
public:
    UPROPERTY(BlueprintAssignable, Category = "AutoTarget | Events")
        FOnHighlightedObjectChanged OnHighlightedObjectChanged;
    UPROPERTY(BlueprintAssignable, Category = "AutoTarget | Events")
        FOnInteractionStarted OnInteractionStarted;
    UPROPERTY(BlueprintAssignable, Category = "AutoTarget | Events")
        FOnInteractionInterrupted OnInteractionInterrupted;
    UPROPERTY(BlueprintAssignable, Category = "AutoTarget | Events")
        FOnActorHighlightedChanged OnActorHighlightedChanged;
private:
    bool bCanTarget = true;
    TMap<AActor*, FMaterialInfo> InteractableObjects;
    AActor* LastSelectedObject;
    APlayerCameraManager* CameraManager;
    class ACharacter* MyPlayer;
};

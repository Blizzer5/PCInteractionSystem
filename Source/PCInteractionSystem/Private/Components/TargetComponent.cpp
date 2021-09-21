// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/TargetComponent.h"
#include "Camera/CameraComponent.h"
#include "CollisionQueryParams.h"
#include "Engine/World.h"
#include "Math/Vector.h"
#include "Engine/Engine.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UTargetComponent::UTargetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Assign some default numeric values to avoid divide-by-zero problem
	if (ArcAngle == 0)
	{
		ArcAngle = 120;
	}

	if (AngleStep == 0)
	{
		AngleStep = 1;
	}

	if (Radius == 0)
	{
		Radius = 500;
	}
}


// Called when the game starts
void UTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(false);

	if (GetOwner()->GetLocalRole() >= ENetRole::ROLE_AutonomousProxy)
	{
		// We need a delay because some clients could not be fully initialized yet
		FTimerHandle InitializationTimer;
		GetWorld()->GetTimerManager().SetTimer(InitializationTimer, this, &UTargetComponent::InitializeComponent, 1.0f, false);
	}
}


void UTargetComponent::InitializeComponent()
{
	MyPlayer = Cast<ACharacter>(GetOwner());
	if (MyPlayer->GetController())
	{
		SetComponentTickEnabled(true);
		MyPlayer->GetController()->GetOnNewPawnNotifier().AddUFunction(this, "OnControllerOwnerChangedPawn");
		if (MyPlayer->InputComponent)
		{
			MyPlayer->InputComponent->BindAction("Interact", IE_Pressed, this, &UTargetComponent::Interact);
			MyPlayer->InputComponent->BindAction("Interact", IE_Released, this, &UTargetComponent::InteractReleased);
		}
		CameraManager = Cast<APlayerController>(MyPlayer->GetController())->PlayerCameraManager;
	}
}

// Called every frame
void UTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bCanTarget)
	{
		GatherObjects(DeltaTime);
	}
}

AActor* UTargetComponent::GetLastHighlightedObject()
{
	return LastSelectedObject;
}

void UTargetComponent::Interact()
{
	if (GetLastHighlightedObject())
	{
		IInteractableInterface::Execute_OnInteractionStarted(LastSelectedObject, MyPlayer);
		OnInteractionStarted.Broadcast();
	}
}

void UTargetComponent::InteractReleased()
{
	if (GetLastHighlightedObject())
	{
		IInteractableInterface::Execute_StopInteraction(LastSelectedObject);
		OnInteractionInterrupted.Broadcast();
	}
}

void UTargetComponent::DisableTargeting()
{
	bCanTarget = false;
}

void UTargetComponent::EnableTargeting()
{
	bCanTarget = true;
}

void UTargetComponent::OnControllerOwnerChangedPawn()
{
	if (MyPlayer->GetController())
	{
		EnableTargeting();
		MyPlayer->InputComponent->BindAction("Interact", IE_Pressed, this, &UTargetComponent::Interact);
		MyPlayer->InputComponent->BindAction("Interact", IE_Released, this, &UTargetComponent::InteractReleased);
	}
	else
	{
		DisableTargeting();
	}
}

void UTargetComponent::GatherObjects(float DeltaSeconds)
{
	UWorld* World = GetWorld();
	if (World == nullptr)
		return;

	InteractableObjects.Empty();

	const FName TraceTag("AutoTargetTraceTag");
	FCollisionQueryParams TraceParams = FCollisionQueryParams(TraceTag, false, nullptr);
	TraceParams.AddIgnoredActor(MyPlayer);

	FVector HitPoint;
	TArray<AActor*> IgnoreActors;

	if (cameraType == ECameraType::ThirdPerson) {
		FVector TraceStartLocation = MyPlayer->GetActorLocation() + MyPlayer->GetActorForwardVector() * 100;
		FVector CurrentActorForward = MyPlayer->GetControlRotation().Vector().ForwardVector;

		TArray<FHitResult> HitResults;

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
		ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2);
		ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);
		ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery4);
		ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery5);
		ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery6);

		if (UKismetSystemLibrary::SphereTraceMultiForObjects(this, TraceStartLocation, TraceStartLocation, 100.f, ObjectTypes, true, IgnoreActors, bShowDebug ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None, HitResults, true))
		{
			for (FHitResult Hit : HitResults)
			{
				if (IInteractableInterface* InteractableActor = Cast<IInteractableInterface>(Hit.Actor))
				{
					if (IInteractableInterface::Execute_IsInteractable(Hit.Actor.Get()) && !InteractableObjects.Contains(Hit.Actor.Get()))
					{
						InteractableObjects.Add(Hit.Actor.Get());
					}
				}
			}
		}
	}
	else if (cameraType == ECameraType::FirstPerson) {
		FVector TraceStartLocation = CameraManager->GetCameraLocation();
		FVector TraceEndLocation = CameraManager->GetCameraLocation() + CameraManager->GetActorForwardVector() * Radius;

		FHitResult HitResult;

		if (UKismetSystemLibrary::LineTraceSingle(this, TraceStartLocation, TraceEndLocation, TraceTypeQuery1, true, IgnoreActors, bShowDebug ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None, HitResult, true)) {
			if (bOnlyInteractableObjects) {
				if (IInteractableInterface* InteractableActor = Cast<IInteractableInterface>(HitResult.Actor))
				{
					if (IInteractableInterface::Execute_IsInteractable(HitResult.Actor.Get()) && !InteractableObjects.Contains(HitResult.Actor.Get()))
					{
						InteractableObjects.Add(HitResult.Actor.Get());
					}
				}
			}
			else {
				InteractableObjects.Add(HitResult.Actor.Get());
			}
		}
	}
	HighlightClosestObject();
}

void UTargetComponent::HighlightClosestObject()
{
	float closestDistance = 99999999;
	AActor* closestObject = nullptr;
	for (uint8 i = 0; i < InteractableObjects.Num(); ++i)
	{
		float ObjectDistance = FVector::Dist(CameraManager->GetCameraLocation(), Cast<AActor>(InteractableObjects[i])->GetActorLocation());
		if (ObjectDistance < closestDistance)
		{
			closestDistance = ObjectDistance;
			closestObject = InteractableObjects[i];
		}
	}

	if (closestObject != LastSelectedObject)
	{
		if (LastSelectedObject)
		{
			if (TScriptInterface<IInteractableInterface> ScriptInteractable = TScriptInterface<IInteractableInterface>(LastSelectedObject)) {
				IInteractableInterface::Execute_OnHighlightRemoved(LastSelectedObject);
			}
		}

		if (closestObject)
		{
			if (TScriptInterface<IInteractableInterface> ScriptInteractable = TScriptInterface<IInteractableInterface>(closestObject)) {
				IInteractableInterface::Execute_OnHighlighted(closestObject);
			}
		}

		if (TScriptInterface<IInteractableInterface> ScriptInteractable = TScriptInterface<IInteractableInterface>(closestObject ? closestObject : nullptr)) {
			OnHighlightedObjectChanged.Broadcast(ScriptInteractable);
		}
		else {
			OnActorHighlightedChanged.Broadcast(closestObject, LastSelectedObject);
		}

		LastSelectedObject = closestObject;
	}
}
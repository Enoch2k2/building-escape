// &copy Enoch Griffith 2017

#include "OpenDoor.h"
#include "./GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "./Classes/Components/PrimitiveComponent.h"

# define OUT
// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if (!PressurePlate) {UE_LOG(LogTemp, Warning, TEXT("There is no pressure plate attached"))}
	if (!Owner) {UE_LOG(LogTemp, Warning, TEXT("There is no Owner Component"))}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll The Trigger Volume
	if (GetTotalMassOfActorsOnPlate() > TriggerMass) { // TODO make into parameter
		OnOpen.Broadcast();
	}
	else {
		// Check if it's time to close the door
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	// Find all the overlapping actors;
	
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OverlappingActors);
		
	// Iterate through them adding their masses;
	for(const auto* Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	};

	return TotalMass;
}
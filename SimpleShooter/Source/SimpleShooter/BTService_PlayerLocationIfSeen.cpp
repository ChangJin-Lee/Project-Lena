// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationIfSeen.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen()
{
	NodeName = TEXT("Update Player Location If Seen");
}


void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	AAIController *AIController =  OwnerComp.GetAIOwner();
	APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(),0);

	if(PlayerPawn == nullptr)
	{
		return;
	}

	if(AIController == nullptr)
	{
		return;
	}

	if(AIController->LineOfSightTo(PlayerPawn))
	{
		AIController->GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), PlayerPawn);
	}
	else
	{
		AIController->GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena/Public/Game/AI/BTService_PlayerLocationIfSeen.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Lena/Public/Characters/Base_Character.h"


UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen()
{
	NodeName = TEXT("Update Player Location If Seen");
}

void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	AAIController *AIController =  OwnerComp.GetAIOwner();
	APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
	
	APawn *AIPawn = AIController->GetPawn();
	ABase_Character *ShooterCharacter = Cast<ABase_Character>(AIPawn);

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
		ShooterCharacter->ToggleIsArmed();
		ShooterCharacter->DrawAIWeapon();
		AIController->GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), PlayerPawn);
	}
	else
	{
		ShooterCharacter->ToggleIsArmed();
		ShooterCharacter->PutDownAIWeapon();
		AIController->GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_ClearBlackBoardValue.h"

#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UMyBTTask_ClearBlackBoardValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent)
	{
		return EBTNodeResult::Failed;
	}

	BlackboardComponent->ClearValue(GetSelectedBlackboardKey());
	return EBTNodeResult::Succeeded;
}

UMyBTTask_ClearBlackBoardValue::UMyBTTask_ClearBlackBoardValue()
{
	NodeName = TEXT("Clear Blackboard Value");
}

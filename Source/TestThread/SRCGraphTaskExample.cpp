// Fill out your copyright notice in the Description page of Project Settings.


#include "SRCGraphTaskExample.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ASRCGraphTaskExample::ASRCGraphTaskExample()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASRCGraphTaskExample::BeginPlay()
{
	Super::BeginPlay();

	ATestThreadGameModeBase *GameModeRef = Cast<ATestThreadGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GameModeRef != nullptr)
	{
		TaskDelegate.BindUFunction(this, FName("OnWorkDone"));
/*		TGraphTask<FTask_GraphTask>::CreateTask(nullptr, ENamedThreads::AnyThread)
			.ConstructAndDispatchWhenReady(TaskDelegate, GameModeRef, &Counter, 321);
*/
		MyCurrentTask = TGraphTask<FTask_GraphTask>::CreateTask(nullptr, ENamedThreads::AnyThread)
			.ConstructAndHold(TaskDelegate, GameModeRef, &Counter1, 321);

	}
	
}

// Called every frame
void ASRCGraphTaskExample::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASRCGraphTaskExample::OnWorkDone(int32 Result)
{
	MyCurrentTask = nullptr;
	OnWorkDoneBP(Result);
}

void ASRCGraphTaskExample::OnWorkDoneBP_Implementation(int32 Result)
{
	// IN BP
}

void ASRCGraphTaskExample::StartAsyncWork()
{
	if (MyCurrentTask != nullptr && !MyCurrentTask->GetCompletionEvent().IsValid())
	{
		MyCurrentTask->Unlock();
	}
	else
	{
		ATestThreadGameModeBase *GameModeRef = Cast<ATestThreadGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

		if (GameModeRef != nullptr)
		{
			TGraphTask<FTask_GraphTask>::CreateTask(nullptr, ENamedThreads::AnyThread)
				.ConstructAndDispatchWhenReady(TaskDelegate, GameModeRef, &Counter1, 321);
		}
	}
}


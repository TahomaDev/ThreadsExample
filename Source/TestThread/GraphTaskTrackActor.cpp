// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphTaskTrackActor.h"

// Sets default values
AGraphTaskTrackActor::AGraphTaskTrackActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AGraphTaskTrackActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CounterStopFlag = true;
	for (auto It : Prerequisites)
	{
		if (It.IsValid() && !It->IsComplete())
		{
			It->Wait();
		}
	}
	Super::EndPlay(EndPlayReason);
}

void AGraphTaskTrackActor::StartGraphTaskTrack()
{
	for (int i = 0; i < 6; i++)
	{
		FGraphEventRef CounterTask = TGraphTask<FTask_Counter_Track>::CreateTask(nullptr, ENamedThreads::GameThread).ConstructAndDispatchWhenReady(&Counter, &CounterMutex, &CounterStopFlag);
		Prerequisites.Add(CounterTask);
	}
	TaskDelefate_OnWorkDone.BindWeakLambda(this, [this](int32 Result) {
		OnWorkDoneBP(Result);
	});
	TGraphTask<FTask_FinishWork_Track>::CreateTask(&Prerequisites, ENamedThreads::AnyThread).ConstructAndDispatchWhenReady(Counter, TaskDelefate_OnWorkDone);
}

void AGraphTaskTrackActor::OnWorkDoneBP_Implementation(int32 Result)
{
	//IN BP
}


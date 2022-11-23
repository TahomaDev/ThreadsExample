// Fill out your copyright notice in the Description page of Project Settings.


#include "SRCAsyncTask.h"

// Sets default values
ASRCAsyncTask::ASRCAsyncTask()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASRCAsyncTask::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASRCAsyncTask::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASRCAsyncTask::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ASRCAsyncTask::StartScopeAsyncTask()
{
	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [&]()
	{
		uint32 i = 0;
		while (i < 500)
		{
			FPlatformProcess::Sleep(0.02f);
			i++;
			Counter2 = i;
		}
	});
}

void ASRCAsyncTask::StartAsyncTaskExample(bool bIsBackgroundTask)
{
	if (bIsBackgroundTask)
	{
		(new FAutoDeleteAsyncTask<FAsyncTask_Counter>(5, AsyncTaskDelegate_OnWorkDone, &Counter))->StartBackgroundTask();
	}
	else
	{
		(new FAutoDeleteAsyncTask<FAsyncTask_Counter>(5, AsyncTaskDelegate_OnWorkDone, &Counter))->StartSynchronousTask();
	}
}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestThreadGameModeBase.h"
#include "GameFramework/Actor.h"
#include "SRCGraphTaskExample.generated.h"

DECLARE_DELEGATE_OneParam(FTaskDelegate_OnWorkDone, int32 OutputResult);

#pragma optimize("", off) // only for breakpoints
class FTask_FinishWork
{
	FTaskDelegate_OnWorkDone TaskDelegate_OnWorkDone;
	ATestThreadGameModeBase* GameModeRef;
	int32 Result;
	
public:
	FTask_FinishWork(FTaskDelegate_OnWorkDone InTaskDelegate_OnWorkDone, ATestThreadGameModeBase* InGameModeRef, int32 InSimpleOutput) // CAUTION!: Must not use references in the constructor args; use pointers instead if you need by reference
		: TaskDelegate_OnWorkDone(InTaskDelegate_OnWorkDone), GameModeRef(InGameModeRef), Result(InSimpleOutput)
	
	{
		// Usually the constructor doesn't do anything except save the arguments for use in DoWork or GetDesiredThread.
		UE_LOG(LogTemp, Warning, TEXT("FTask_FinishWork constructor"));
	}
	~FTask_FinishWork()
	{
		// you will be destroyed immediately after you execute. Might as well do cleanup in DoWork, but you could also use a destructor.
		UE_LOG(LogTemp, Warning, TEXT("~FTask_FinishWork destructor"));
	}
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FGenericTask, STATGROUP_TaskGraphTasks);
	}

	static ENamedThreads::Type GetDesiredThread()
	{

		return ENamedThreads::GameThread;
	}

	void ThreadWork()
	{

	}

	static ESubsequentsMode::Type GetSubsequentsMode() { return ESubsequentsMode::FireAndForget; };
	
	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
	{
		check(IsInGameThread())
		if (TaskDelegate_OnWorkDone.IsBound())
		{
			TaskDelegate_OnWorkDone.Execute(Result);
		}
		
	}
};

class FTask_GraphTask
{
	FTaskDelegate_OnWorkDone TaskDelegate_OnWorkDone;
	ATestThreadGameModeBase* GameModeRef;
	int32 *SimpleOutput;
	int32 Number;
	
public:
	FTask_GraphTask(FTaskDelegate_OnWorkDone InTaskDelegate_OnWorkDone, ATestThreadGameModeBase* InGameModeRef, int32 *InSimpleOutput, int32 InNumber) // CAUTION!: Must not use references in the constructor args; use pointers instead if you need by reference
		: TaskDelegate_OnWorkDone(InTaskDelegate_OnWorkDone), GameModeRef(InGameModeRef), SimpleOutput(InSimpleOutput), Number(InNumber)
	
	{
		// Usually the constructor doesn't do anything except save the arguments for use in DoWork or GetDesiredThread.
		UE_LOG(LogTemp, Warning, TEXT("FTask_GraphTask constructor"));
	}
	~FTask_GraphTask()
	{
		// you will be destroyed immediately after you execute. Might as well do cleanup in DoWork, but you could also use a destructor.
		UE_LOG(LogTemp, Warning, TEXT("~FTask_GraphTask destructor"));
	}
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FGenericTask, STATGROUP_TaskGraphTasks);
	}

	static ENamedThreads::Type GetDesiredThread()
	{
		 FAutoConsoleTaskPriority CPrio_MyGraphAsyncTask(
		TEXT("TaskGraph.Test.ExampleTask"),
		TEXT("TaskGraph.Test.ExampleTask help text"),
			ENamedThreads::BackgroundThreadPriority, // if we have background priority task threads, then use them...
			ENamedThreads::NormalTaskPriority, // .. at normal task priority
			ENamedThreads::NormalTaskPriority // if we don't have background threads, then use normal priority threads at normal task priority instead
			);
		
		return CPrio_MyGraphAsyncTask.Get(); // ENamedThreads::AnyBackgroundThreadNormalTask;
	}

	void ThreadWork()
	{
		int32 i = 0;
		while (i < 500)
		{
			FPlatformProcess::Sleep(0.001f);
			*SimpleOutput = (*SimpleOutput + 1);
			i++;
		}
	}

	static ESubsequentsMode::Type GetSubsequentsMode() { return ESubsequentsMode::FireAndForget; };
	
	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
	{
		ThreadWork();
		TGraphTask<FTask_FinishWork>::CreateTask(nullptr, CurrentThread).ConstructAndDispatchWhenReady(TaskDelegate_OnWorkDone, GameModeRef, *SimpleOutput);
		
		// The arguments are useful for setting up other tasks. 
		// Do work here, probably using SomeArgument.
		//MyCompletionGraphEvent->DontCompleteUntil(TGraphTask<FSomeChildTask>::CreateTask(NULL,CurrentThread).ConstructAndDispatchWhenReady());
	}
};
#pragma optimize("", on)

UCLASS()
class TESTTHREAD_API ASRCGraphTaskExample : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASRCGraphTaskExample();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	void OnWorkDoneBP(int32 Result);
	UFUNCTION()
	void OnWorkDone(int32 Result);
	UFUNCTION(BlueprintCallable)
	void StartAsyncWork();

	UPROPERTY(BlueprintReadWrite)
	int32 Counter = 0;
	FTaskDelegate_OnWorkDone TaskDelegate;

	TGraphTask<FTask_GraphTask> *MyCurrentTask;

};


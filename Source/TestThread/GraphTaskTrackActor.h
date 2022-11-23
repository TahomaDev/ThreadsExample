// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GraphTaskTrackActor.generated.h"

DECLARE_DELEGATE_OneParam(FTaskDelegate_OnWorkDone, int32 OutputResult);

class FTask_FinishWork_Track
{
	int32 Result;
	FTaskDelegate_OnWorkDone TaskDelegate_OnWorkDone;

public:
	FTask_FinishWork_Track(int32 InResult, FTaskDelegate_OnWorkDone InTaskDelegate_OnWorkDone) : Result(InResult), TaskDelegate_OnWorkDone(InTaskDelegate_OnWorkDone) {}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FTask_FinishWork_Track, STATGROUP_TaskGraphTasks);
	}

	static ENamedThreads::Type GetDesiredThread()
	{
		return ENamedThreads::GameThread;
	}

	static ESubsequentsMode::Type GetSubsequentsMode() { return ESubsequentsMode::TrackSubsequents; }

	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
	{
		if (TaskDelegate_OnWorkDone.IsBound())
		{
			TaskDelegate_OnWorkDone.Execute(Result);
		}
	};
};

class FTask_Counter_Track
{
	int32 *Result;
	FCriticalSection *CounterMutex;
	FThreadSafeBool *CounterStopFlag;

public:
	FTask_Counter_Track(int32 *InResult, FCriticalSection *InCounterMutex, FThreadSafeBool *InCounterStopFlag) : Result(InResult), CounterMutex(InCounterMutex), CounterStopFlag(InCounterStopFlag) {}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FTask_Counter_Track, STATGROUP_TaskGraphTasks);
	}

	static ENamedThreads::Type GetDesiredThread()
	{
		FAutoConsoleTaskPriority TaskPriority(
		TEXT("TaskGraph.Track.ExampleCounter"),
		TEXT("TaskGraph.Track.ExampleCounter help text"),
			ENamedThreads::BackgroundThreadPriority, // if we have background priority task threads, then use them...
			ENamedThreads::NormalTaskPriority, // .. at normal task priority
			ENamedThreads::NormalTaskPriority // if we don't have background threads, then use normal priority threads at normal task priority instead
			);
		return TaskPriority.Get();
	}

	static ESubsequentsMode::Type GetSubsequentsMode() { return ESubsequentsMode::TrackSubsequents; }

	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
	{
		for (int i = 0; i < 50; ++i)
		{
			if (*CounterStopFlag)
			{
				break;
			}
			FPlatformProcess::Sleep(0.01f);
			FScopeLock ScopeLock(CounterMutex);
			*Result = *Result + 1;
		}
	};
};


UCLASS()
class TESTTHREAD_API AGraphTaskTrackActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGraphTaskTrackActor();
	
protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	UFUNCTION(BlueprintCallable)
	void StartGraphTaskTrack();

	UFUNCTION(BlueprintNativeEvent)
	void OnWorkDoneBP(int32 Result);

	FGraphEventArray Prerequisites;
	
	UPROPERTY(BlueprintReadOnly)
	int32 Counter = 0;

	FCriticalSection CounterMutex;
	FThreadSafeBool CounterStopFlag = false;
	FTaskDelegate_OnWorkDone TaskDelefate_OnWorkDone;

};

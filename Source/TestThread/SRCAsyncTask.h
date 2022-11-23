// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRCAsyncTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAsyncTaskDelegate_OnWorkDone, int32, OutputResult);

class FAsyncTask_Counter : public FNonAbandonableTask
{
	friend class FAutoDeleteAsyncTask<FAsyncTask_Counter>;

	int32 ExampleData;
	FAsyncTaskDelegate_OnWorkDone AsyncTaskDelegate_OnWorkDone;
	int32 *SimpleCounter;

	FAsyncTask_Counter(int32 InExampleData, FAsyncTaskDelegate_OnWorkDone InAsyncTaskDelegate_OnWorkDone, int32 *InSimpleCounter)
		: ExampleData(InExampleData), AsyncTaskDelegate_OnWorkDone(InAsyncTaskDelegate_OnWorkDone), SimpleCounter(InSimpleCounter)
	{
	}

	void DoWork()
	{
		uint32 i = 0;
		while (i < 500)
		{
			FPlatformProcess::Sleep(0.001f);
			*SimpleCounter = (*SimpleCounter + 1);
			i++;
		}
	}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FAsyncTask_Counter, STATGROUP_ThreadPoolAsyncTasks);
	}

	
	
};

UCLASS()
class TESTTHREAD_API ASRCAsyncTask : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASRCAsyncTask();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UFUNCTION(BlueprintCallable)
	void StartAsynTaskExample(bool bIsBackgroundTask = true);

	UPROPERTY(BlueprintAssignable)
	FAsyncTaskDelegate_OnWorkDone AsyncTaskDelegate_OnWorkDone;
	UPROPERTY(BlueprintReadWrite)
	int32 Counter = 0;
	UPROPERTY(BlueprintReadWrite)
	int32 Counter2 = 0;

};

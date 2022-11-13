// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HAL/ThreadingBase.h"

#include "TestThreadGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TESTTHREAD_API ATestThreadGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(BlueprintReadWrite, Category="SimpleAtomic Settings")
	int32 IteractionRunnableCircle = 100000;
	UPROPERTY(BlueprintReadWrite, Category="SimpleAtomic Settings")
	int32 NumberOfThreadToCreate = 12;
	UPROPERTY(BlueprintReadWrite, Category="SimpleAtomic Settings")
	bool bUseAtomic = true;

	//SimpleCounter Settings
	UPROPERTY(BlueprintReadWrite)
	bool bIsUseSafeVariable = true;
	
	class FSimpleCounter_Runnable* MyRunnableClass_SimpleCounter = nullptr;
	FRunnableThread* CurrentRunningGameModeThread_SimpleCounter = nullptr;
	
	//SimpleCounter control
	UFUNCTION(BlueprintCallable)
	void CreateSimpleCounterThread();
	UFUNCTION(BlueprintCallable)
	void StopSimpleCounterThread();
	UFUNCTION(BlueprintCallable)
	void KillSimpleCounterThread(bool bIsShouldWait);
	UFUNCTION(BlueprintCallable)
	int64 GetSimpleCounterThread();
	UFUNCTION(BlueprintCallable)
	bool SwitchRunStateSimpleCounterThread(bool bIsPause);


	
	//SimpleAtomic Settings
	TArray<FRunnableThread*> CurrentRunnableGameMode_SimpleAtomic;
	
	//SimpleAtomic control
	UFUNCTION(BlueprintCallable)
	void CreateSimpleAtomicThread();
	UFUNCTION(BlueprintCallable)
	void GetCounterSimpleAtomic(int32 &Atomic1, int32 &Atomic2, int32 &NonAtomic1, int32 &NonAtomic2);
	UFUNCTION(BlueprintCallable)
	void ResetCounterSimpleAtomicThread();
	
	//SimpleAtomic storage
	std::atomic_int16_t AtomicCounter1;
	std::atomic_int16_t AtomicCounter2;
	int16 NotAtomicCounter1;
	int16 NotAtomicCounter2;
	
};

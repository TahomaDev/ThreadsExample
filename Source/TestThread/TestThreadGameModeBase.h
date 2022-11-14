// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MessageEndpoint.h"
#include "GameFramework/GameModeBase.h"
#include "HAL/ThreadingBase.h"

#include "TestThreadGameModeBase.generated.h"

USTRUCT(BlueprintType)
struct FInfoNPC
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Id = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name = "None";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SecondName = "None";
};

USTRUCT(BlueprintType)
struct FBusStructMessage_NameGenerator
{
	GENERATED_BODY()

	bool bIsSecondName = false;
	FString TextData = "None";

	FBusStructMessage_NameGenerator(bool InBool = false, FString InText = "None") : bIsSecondName(InBool), TextData(InText) {}  
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateByNameGeneratorThreads, bool, bIsSecond, FString, StringData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateByNPCThread, FInfoNPC, DataNPC);

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

	//IBusMessage
	UPROPERTY(BlueprintAssignable)
	FOnUpdateByNameGeneratorThreads OnUpdateByNameGeneratorThreads;
	UPROPERTY(BlueprintAssignable)
	FOnUpdateByNPCThread OnUpdateByNPCThread;
	TSharedPtr<FMessageEndpoint, ESPMode::ThreadSafe> ReceiveEndPoint_NameGenerator;
	TSharedPtr<FMessageEndpoint, ESPMode::ThreadSafe> ReceiveEndPoint_NPCInfo;
	void BusMessageHandler_NameGenerator(const FBusStructMessage_NameGenerator& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context);
	void BusMessageHandler_NPCInfo(const FInfoNPC& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context);
	void EventMessage_NameGenerator(bool bIsSecondName, FString StringData);
	void EventMessage_NPCInfo(FInfoNPC NPCData);


	

	UPROPERTY(BlueprintReadWrite, Category="SimpleAtomic Settings")
	int32 IteractionRunnableCircle = 100000;
	UPROPERTY(BlueprintReadWrite, Category="SimpleAtomic Settings")
	int32 NumberOfThreadToCreate = 12;
	UPROPERTY(BlueprintReadWrite, Category="SimpleAtomic Settings")
	bool bUseAtomic = true;

	//SimpleCounter Settings
	UPROPERTY(BlueprintReadWrite)
	bool bIsUseSafeVariable = true;
	UPROPERTY(BlueprintReadWrite)
	bool bIsUseFEvent = true;
	UPROPERTY(BlueprintReadWrite)
	bool bIsUseFScopedEvent = true;

	class FSimpleCounter_Runnable* MyRunnableClass_SimpleCounter = nullptr;
	FRunnableThread* CurrentRunningGameModeThread_SimpleCounter = nullptr;
	FEvent* SimpleCounterEvent;
	FScopedEvent* SimpleCounterScopedEvent_Ref;
	
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

	UFUNCTION(BlueprintCallable)
	void StartSimpleCounterThreadWithEvent();
	UFUNCTION(BlueprintCallable)
	void StartSimpleCounterThreadWithScopedEvent();

	void SendRef_ScopedEvent(FScopedEvent &ScopedEvent_Ref);



	
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



	
	//SimpleMutex settings
	TArray<FRunnableThread*> CurrentRunningGameModeThread_SimpleMutex;
	FRunnableThread* CurrentRunningGameModeThread_SimpleCollectable;

	//SimpleMutex control
	UFUNCTION(BlueprintCallable)
	void CreateSimpleMutexThread();
	UFUNCTION(BlueprintCallable)
	void CreateSimpleCollectableThread();
	UFUNCTION(BlueprintCallable)
	void StopSimpleMutexThreads();
	UFUNCTION(BlueprintCallable)
	TArray<FString> GetSecondNames();
	UFUNCTION(BlueprintCallable)
	TArray<FString> GetFirstNames();
	UFUNCTION(BlueprintCallable)
	TArray<FInfoNPC> GetNPCInfo();
	
	//SimpleMutex storage
	TArray<FString> FirstNames;
	FCriticalSection FirstNameMutex;

	TQueue<FString, EQueueMode::Mpsc> SecondName;
	TArray<FString> CurrentSecondName;
};

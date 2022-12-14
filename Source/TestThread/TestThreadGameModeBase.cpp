// Copyright Epic Games, Inc. All Rights Reserved.


#include "TestThreadGameModeBase.h"

#include "MessageEndpointBuilder.h"
#include "SRCBoxActor.h"
#include "SimPrim/SimpleAtomic_Runnable.h"
#include "SimPrim/SimpleCounter_Runnable.h"
#include "SimPrim/SimpleMutex_Runnable.h"
#include "SimPrim/SimpleCollectable_Runnable.h"



void ATestThreadGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

/*	const uint32 ThreadID = FPlatformTLS::GetCurrentThreadId();
	UE_LOG(LogTemp, Warning, TEXT("ThreadID: %d, Name: %s"), ThreadID, *FThreadManager::GetThreadName(ThreadID));

	FThreadManager::Get().ForEachThread([&](uint32 ThreadId, FRunnableThread* Runnable)
	{
		Runnable->GetThreadPriority()
	});*/
}

void ATestThreadGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//IBusMessage
	ReceiveEndPoint_NameGenerator = FMessageEndpoint::Builder("Receiver_ATestThreadGameModeBase").Handling<FBusStructMessage_NameGenerator>(this, &ATestThreadGameModeBase::BusMessageHandler_NameGenerator);
	if (ReceiveEndPoint_NameGenerator.IsValid())
	{
		ReceiveEndPoint_NameGenerator->Subscribe<FBusStructMessage_NameGenerator>();
	}

	ReceiveEndPoint_NPCInfo = FMessageEndpoint::Builder("Receiver_ATestThreadGameModeBaseNPC").Handling<FInfoNPC>(this, &ATestThreadGameModeBase::BusMessageHandler_NPCInfo);
	if (ReceiveEndPoint_NPCInfo.IsValid())
	{
		ReceiveEndPoint_NPCInfo->Subscribe<FInfoNPC>();
	}
}


void ATestThreadGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	StopSimpleCounterThread();
	StopSimpleMutexThreads();

	//IBusMessage
	if (ReceiveEndPoint_NameGenerator.IsValid())
		ReceiveEndPoint_NameGenerator.Reset();
}



void ATestThreadGameModeBase::CreateSimpleAtomicThread()
{
	for(int i = 0; i < NumberOfThreadToCreate; i++)
	{
		bool bIsFlag = (i%2) ? true : false;
		FColor Color = FColor::Yellow;
		class FSimpleAtomic_Runnable *MyRunnableClass_SimpleAtomic = new FSimpleAtomic_Runnable(Color, this, IteractionRunnableCircle, bIsFlag, bUseAtomic);
		CurrentRunnableGameMode_SimpleAtomic.Add(FRunnableThread::Create(MyRunnableClass_SimpleAtomic, TEXT("SimpleAtomic thread"), 0, EThreadPriority::TPri_Normal));
	}
}


void ATestThreadGameModeBase::GetCounterSimpleAtomic(int32& Atomic1, int32& Atomic2, int32& NonAtomic1,
	int32& NonAtomic2)
{
	if (bUseAtomic)
	{
		if (AtomicCounter1.is_lock_free()) { Atomic1 = AtomicCounter1.load(); }
		if (AtomicCounter2.is_lock_free()) { Atomic2 = AtomicCounter2.load(); }
		NonAtomic1 = FPlatformAtomics::AtomicRead(&NotAtomicCounter1);
		NonAtomic2 = FPlatformAtomics::AtomicRead(&NotAtomicCounter2);
	}
	else
	{
		NonAtomic1 = NotAtomicCounter1;
		NonAtomic2 = NotAtomicCounter2;
	}
	
}


void ATestThreadGameModeBase::ResetCounterSimpleAtomicThread()
{
	AtomicCounter1 = 0;
	AtomicCounter2 = 0;
	NotAtomicCounter1 = 0;
	NotAtomicCounter2 = 0;
}


void ATestThreadGameModeBase::CreateSimpleCounterThread()
{
	if (bIsUseFEvent)
	{
		SimpleCounterEvent = FPlatformProcess::GetSynchEventFromPool();
	}
	
	if (CurrentRunningGameModeThread_SimpleCounter == nullptr && MyRunnableClass_SimpleCounter == nullptr)
	{
		const FColor Color = FColor::Cyan;
		MyRunnableClass_SimpleCounter = new FSimpleCounter_Runnable(Color, this, bIsUseSafeVariable);
		CurrentRunningGameModeThread_SimpleCounter = FRunnableThread::Create(MyRunnableClass_SimpleCounter, TEXT("SimpleCounter Thread"), 0, EThreadPriority::TPri_Normal);
	}
}


void ATestThreadGameModeBase::StopSimpleCounterThread()
{
	if (CurrentRunningGameModeThread_SimpleCounter != nullptr)
	{
		if (MyRunnableClass_SimpleCounter)
		{
			MyRunnableClass_SimpleCounter->bIsStopThread = true;
			MyRunnableClass_SimpleCounter->bIsStopThreadSafe = true;
			CurrentRunningGameModeThread_SimpleCounter->Suspend(false);

			if (SimpleCounterEvent)
			{
				SimpleCounterEvent->Trigger();
				FPlatformProcess::ReturnSynchEventToPool(SimpleCounterEvent);
				SimpleCounterEvent = nullptr;
			}

			if (SimpleCounterScopedEvent_Ref)
			{
				SimpleCounterScopedEvent_Ref->Trigger();
				SimpleCounterScopedEvent_Ref = nullptr;
			}
			
			CurrentRunningGameModeThread_SimpleCounter->WaitForCompletion();
			CurrentRunningGameModeThread_SimpleCounter = nullptr;
			MyRunnableClass_SimpleCounter = nullptr;
		}
	}
}

void ATestThreadGameModeBase::KillSimpleCounterThread(bool bIsShouldWait)
{
	if (CurrentRunningGameModeThread_SimpleCounter != nullptr)
	{
		CurrentRunningGameModeThread_SimpleCounter->Suspend(false);
		CurrentRunningGameModeThread_SimpleCounter->Kill(bIsShouldWait);
		delete CurrentRunningGameModeThread_SimpleCounter;
		CurrentRunningGameModeThread_SimpleCounter = nullptr;
		MyRunnableClass_SimpleCounter = nullptr;
	}
}

int64 ATestThreadGameModeBase::GetSimpleCounterThread()
{
	int64 Result = -1;

	if (CurrentRunningGameModeThread_SimpleCounter != nullptr)
	{
		Result = (MyRunnableClass_SimpleCounter->bIsUseSafeVariable) ? MyRunnableClass_SimpleCounter->CounterSafe.GetValue() : Result = MyRunnableClass_SimpleCounter->Counter;
	}
	return Result;
}

bool ATestThreadGameModeBase::SwitchRunStateSimpleCounterThread(bool bIsPause)
{
	if (CurrentRunningGameModeThread_SimpleCounter != nullptr)
	{
		CurrentRunningGameModeThread_SimpleCounter->Suspend(!bIsPause);
	}

	return !bIsPause;
}

void ATestThreadGameModeBase::StartSimpleCounterThreadWithEvent()
{
	if (SimpleCounterEvent)
	{
		SimpleCounterEvent->Trigger();
		//SimpleCounterEvent = nullptr;
	}
}

void ATestThreadGameModeBase::StartSimpleCounterThreadWithScopedEvent()
{
	if (SimpleCounterScopedEvent_Ref)
	{
		SimpleCounterScopedEvent_Ref->Trigger();
		SimpleCounterScopedEvent_Ref = nullptr;
	}
}

void ATestThreadGameModeBase::SendRef_ScopedEvent(FScopedEvent& ScopedEvent_Ref)
{
	SimpleCounterScopedEvent_Ref = &ScopedEvent_Ref;
}




void ATestThreadGameModeBase::CreateSimpleMutexThread()
{
	FColor Color = FColor::Silver;
	
	for (int i = 0; i < 4; i++)
	{
		bool bFlag = (i%2) ? true : false;

		class FSimpleMutex_Runnable* SimpleMutex_Runnable = new FSimpleMutex_Runnable(Color, this, bFlag);
		CurrentRunningGameModeThread_SimpleMutex.Add(FRunnableThread::Create(SimpleMutex_Runnable, TEXT("SimpleMutex Thread"), 0, EThreadPriority::TPri_Normal));
		
	}
}

void ATestThreadGameModeBase::CreateSimpleCollectableThread()
{
	FColor Color = FColor::Cyan;
	class FSimpleCollectable_Runnable* SimpleCollectable_Runnable = new FSimpleCollectable_Runnable(Color, this);
	CurrentRunningGameModeThread_SimpleCollectable = FRunnableThread::Create(SimpleCollectable_Runnable, TEXT("SimpleCollectable Thread"), 0, EThreadPriority::TPri_Normal);
}

void ATestThreadGameModeBase::StopSimpleMutexThreads()
{
	if (CurrentRunningGameModeThread_SimpleMutex.Num() > 0)
	{
		for (auto RunnableThread : CurrentRunningGameModeThread_SimpleMutex)
		{
			if (RunnableThread != nullptr)
			{
				RunnableThread->Kill(true);
			}
		}
		CurrentRunningGameModeThread_SimpleMutex.Empty();
	}
	
	if (CurrentRunningGameModeThread_SimpleCollectable != nullptr)
	{
		CurrentRunningGameModeThread_SimpleCollectable->Kill(true);
		CurrentRunningGameModeThread_SimpleCollectable = nullptr;
	}
}

TArray<FString> ATestThreadGameModeBase::GetSecondNames()
{
	TArray<FString> Result;
	FString ReadSecondName;
	while (SecondName.Dequeue(ReadSecondName))
	{
		Result.Add(ReadSecondName);
	}

	CurrentSecondName.Append(Result);
	
	return CurrentSecondName;
}

TArray<FString> ATestThreadGameModeBase::GetFirstNames()
{
//	TArray<FString> Result;
//	Result = FirstNames;
//	return Result;
	{
		FScopeLock ScopedFirstNameLocker(&FirstNameMutex);
		return FirstNames;
	}
}

TArray<FInfoNPC> ATestThreadGameModeBase::GetNPCInfo()
{
	TArray<FInfoNPC> Result;
	return Result;
}


void ATestThreadGameModeBase::BusMessageHandler_NameGenerator(const FBusStructMessage_NameGenerator& Message,
                                                              const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context)
{
	EventMessage_NameGenerator(Message.bIsSecondName, Message.TextData);
}

void ATestThreadGameModeBase::BusMessageHandler_NPCInfo(const FInfoNPC& Message,
	const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context)
{
	EventMessage_NPCInfo(Message);
}

void ATestThreadGameModeBase::EventMessage_NameGenerator(bool bIsSecondName, FString StringData)
{
	OnUpdateByNameGeneratorThreads.Broadcast(bIsSecondName, StringData);
}

void ATestThreadGameModeBase::EventMessage_NPCInfo(FInfoNPC NPCData)
{
	OnUpdateByNPCThread.Broadcast(NPCData);

	UWorld *MyWorld = GetWorld();
	if (MyWorld != nullptr && ActorSpawn != nullptr)
	{
		float SpawnStep = (NPCData.Id > 1) ? 60.f : 120.f;
		int Direction = (NPCData.Id % 2) ? 1 : -1;
		FVector SpawnLocation = FVector(1200.f, SpawnStep * NPCData.Id * Direction, 50.f); 
		FRotator SpawnRotator = FRotator::ZeroRotator;
		FActorSpawnParameters SpawnParameters;
		ASRCBoxActor *BoxActor = Cast<ASRCBoxActor>(MyWorld->SpawnActor(ActorSpawn, &SpawnLocation, &SpawnRotator, SpawnParameters));
		if (BoxActor != nullptr)
			BoxActor->Init(NPCData);
	}
}


void ATestThreadGameModeBase::StartParallel1()
{
	FCriticalSection ParallelMutex;
	ParallelFor(10, [&](int32 Index)
	{
		FPlatformProcess::Sleep(0.001f);
		int32 cout = 0;
		for (int i = 0; i < 50; i++)
		{
			cout++;
		}
		ParallelMutex.Lock();
		ParallelCount1 += cout;
		ParallelMutex.Unlock();
		
	}, EParallelForFlags::BackgroundPriority);
}

void ATestThreadGameModeBase::StartParallel2()
{
	FCriticalSection ParallelMutex;
	const auto Function = [&](int32 Index)
	{
		FPlatformProcess::Sleep(0.1f);
		int32 cout = 0;
		for (int i = 0; i < 50; i++)
		{
			cout++;
		}
		ParallelMutex.Lock();
		ParallelCount2 += cout;
		ParallelMutex.Unlock();
	};
	
	ParallelForTemplate(10, Function, EParallelForFlags::BackgroundPriority);
}

void ATestThreadGameModeBase::StartParallel3()
{
	ParallelForWithPreWork(10, [&](int32 Index)
	{
		for (int i = 0; i < 50; i++)
		{
			++ParallelCount3;
			UE_LOG(LogTemp, Error, TEXT("i start ParallelForWithPreWork"));
		}
	}, []()
	{
		UE_LOG(LogTemp, Error, TEXT("i start Help Work"));
//		FPlatformProcess::Sleep(5.f);
		UE_LOG(LogTemp, Error, TEXT("i finish Help Work"));
	}, EParallelForFlags::BackgroundPriority);
}

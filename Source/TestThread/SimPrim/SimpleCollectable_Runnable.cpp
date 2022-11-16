#include "SimpleCollectable_Runnable.h"

#include "MessageEndpointBuilder.h"


FSimpleCollectable_Runnable::FSimpleCollectable_Runnable(FColor NewColor, ATestThreadGameModeBase* OwnerActor)
{
	Color = NewColor;
	Owner = OwnerActor;
	SenderEndPoint = FMessageEndpoint::Builder("Sender_FSimpleCollectable_Runnable").Build();
}

FSimpleCollectable_Runnable::~FSimpleCollectable_Runnable()
{
}

uint32 FSimpleCollectable_Runnable::Run()
{
	uint32 i = 0;
	while (!bIsStopCollectable)
	{
		FPlatformProcess::Sleep(2.0f);
		
		FInfoNPC NewInfoNPC;
		NewInfoNPC.Id = i;

		int32 SizeNameArray = Owner->FirstNames.Num();
		if (SizeNameArray > 0)
		{
			int32 RandNameIndex = FMath::RandHelper(SizeNameArray - 1);
			Owner->FirstNameMutex.Lock();
			NewInfoNPC.Name = Owner->FirstNames[RandNameIndex];
			Owner->FirstNames.RemoveAt(RandNameIndex);
			Owner->FirstNameMutex.Unlock();
		}

		TArray<FString> AvailableSecondNames = Owner->GetSecondNames();
		int32 SizeSecondNameArray = AvailableSecondNames.Num();
		if (SizeSecondNameArray > 0)
		{
			int32 RandNameIndex = FMath::RandHelper(SizeSecondNameArray - 1);
			NewInfoNPC.SecondName = AvailableSecondNames[RandNameIndex];
			AvailableSecondNames.RemoveAt(RandNameIndex);
		}

		{
			FScopeLock NpcScopedLock(&Owner->NPCInfoMutex);
			Owner->NPCInfo.Add(NewInfoNPC);
		}

		if (SenderEndPoint.IsValid())
		{
			SenderEndPoint->Publish<FInfoNPC>(new FInfoNPC(NewInfoNPC));
		}
		
		i++;
	}
	return 1;
}

void FSimpleCollectable_Runnable::Stop()
{
	bIsStopCollectable = true;
}


void FSimpleCollectable_Runnable::Exit()
{
	if (SenderEndPoint.IsValid())
	{
		SenderEndPoint.Reset();
	}
}

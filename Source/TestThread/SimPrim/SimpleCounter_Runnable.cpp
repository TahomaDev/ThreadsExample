#include "SimpleCounter_Runnable.h"

FSimpleCounter_Runnable::FSimpleCounter_Runnable(FColor NewColor, ATestThreadGameModeBase* OwnerActor, bool VariableMode)
{
	Color = NewColor;
	Owner = OwnerActor;
	bIsUseSafeVariable = VariableMode;
}

FSimpleCounter_Runnable::~FSimpleCounter_Runnable()
{
}

bool FSimpleCounter_Runnable::Init()
{
	return true;
}

//#pragma optimize("", off)
uint32 FSimpleCounter_Runnable::Run()
{
	//FScopedEvent
	if (Owner->bIsUseFScopedEvent)
	{
		{
			FScopedEvent SimpleCounterScopedEvent;
			Owner->SendRef_ScopedEvent(SimpleCounterScopedEvent);
		}
	}

	//FEvent
	if (Owner->SimpleCounterEvent)
	{
		Owner->SimpleCounterEvent->Wait(10000);
	}

	
	if (bIsUseSafeVariable)
	{
		while (!bIsStopThreadSafe)
		{
			CounterSafe.Increment();
		}
	}
	else
	{
		while (!bIsStopThread)
		{
		
			Counter++;
		}
	}
	return 1;
}
//#pragma optimize("", on)

void FSimpleCounter_Runnable::Stop()
{
	bIsStopThread = true;
	bIsStopThreadSafe = true;
}

void FSimpleCounter_Runnable::Exit()
{
	Owner = nullptr;
}

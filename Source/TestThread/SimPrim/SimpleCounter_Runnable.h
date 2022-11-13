#pragma once

#include "CoreMinimal.h"
#include "TestThread/TestThreadGameModeBase.h"

class TESTTHREAD_API FSimpleCounter_Runnable : public FRunnable
{
public:
	FSimpleCounter_Runnable(FColor NewColor, ATestThreadGameModeBase *OwnerActor, bool VariableMode);
	virtual ~FSimpleCounter_Runnable() override;

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

	FColor Color;
	ATestThreadGameModeBase* Owner;
	bool bIsUseSafeVariable = true;
	bool bIsStopThread = false;

	// Safe variables
	FThreadSafeBool bIsStopThreadSafe = false;
	FThreadSafeCounter CounterSafe = 0;

	// Not safe variable
	int64 Counter = 0;
};

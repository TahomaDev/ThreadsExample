#pragma once

#include "CoreMinimal.h"
#include "TestThread/TestThreadGameModeBase.h"

class TESTTHREAD_API FSimpleCollectable_Runnable : public FRunnable
{
public:
	FSimpleCollectable_Runnable(FColor NewColor, ATestThreadGameModeBase* OwnerActor);
	virtual ~FSimpleCollectable_Runnable() override;
	
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

	FColor Color;
	ATestThreadGameModeBase* Owner;
	FThreadSafeBool bIsStopCollectable = false;
	TSharedPtr<FMessageEndpoint, ESPMode::ThreadSafe> SenderEndPoint;
	
};

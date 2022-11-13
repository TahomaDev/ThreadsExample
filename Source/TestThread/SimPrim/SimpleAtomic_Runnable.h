#pragma once

#include "CoreMinimal.h"
#include "TestThread/TestThreadGameModeBase.h"


class TESTTHREAD_API FSimpleAtomic_Runnable : public FRunnable
{
public:

	FSimpleAtomic_Runnable(FColor NewColor, ATestThreadGameModeBase *OwnerActor, uint32 NeedIteration, bool SeparateLogic, bool bIsUseAtomic);
	virtual ~FSimpleAtomic_Runnable() override;

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

	FColor Color;
	ATestThreadGameModeBase* Owner;
	int NumberIteration = 0;
	bool bIsStopThread = false;
	bool bUseAtomicFlag = true;
	bool SeparateLoginFlag = false;
};

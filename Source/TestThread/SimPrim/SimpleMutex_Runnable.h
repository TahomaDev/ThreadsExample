#pragma once

#include "CoreMinimal.h"
#include "TestThread/TestThreadGameModeBase.h"

class TESTTHREAD_API FSimpleMutex_Runnable : public FRunnable
{
public:
	FSimpleMutex_Runnable(FColor NewColor, ATestThreadGameModeBase* OwnerActor, bool IsSecondMode);
	virtual ~FSimpleMutex_Runnable() override;

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

	FColor Color;
	ATestThreadGameModeBase* Owner;
	bool bIsGenerateSecondName;
	FThreadSafeBool bIsStopNameGenerator;

	int8 GetRandomInt8(int8 min, int8 max);
	bool GetRandomBool();
	
};

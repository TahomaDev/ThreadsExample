#include "SimpleAtomic_Runnable.h"

FSimpleAtomic_Runnable::FSimpleAtomic_Runnable(FColor NewColor, ATestThreadGameModeBase *OwnerActor, uint32 NeedIteration, bool SeparateLogic, bool bIsUseAtomic)
{
	Color = NewColor;
	Owner = OwnerActor;
	NumberIteration = NeedIteration;
	SeparateLoginFlag = SeparateLogic;
	bUseAtomicFlag = bIsUseAtomic;

	UE_LOG(LogTemp, Warning, TEXT("FSimpleAtomic_Runnable::FSimpleAtomic_Runnable"));
}

FSimpleAtomic_Runnable::~FSimpleAtomic_Runnable()
{
	UE_LOG(LogTemp, Warning, TEXT("FSimpleAtomic_Runnable::~FSimpleAtomic_Runnable"));
}

bool FSimpleAtomic_Runnable::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("FSimpleAtomic_Runnable::Init"));
	return true;
}

uint32 FSimpleAtomic_Runnable::Run()
{
	//FPlatformProcess::Sleep(1.f);
	
	for (int i = 0; i < NumberIteration; i++)
	{
		if (SeparateLoginFlag)
		{
			if (bUseAtomicFlag)
			{
				Owner->AtomicCounter1.fetch_add(1);
				FPlatformAtomics::InterlockedIncrement(&Owner->NotAtomicCounter1);
			}
			else
			{
				Owner->NotAtomicCounter1++;
			}
		}
		else
		{
			if (bUseAtomicFlag)
			{
				Owner->AtomicCounter2.fetch_add(1);
				FPlatformAtomics::InterlockedIncrement(&Owner->NotAtomicCounter2);
			}
			else
			{
				Owner->NotAtomicCounter2++;
			}
		}
	}
	return 1;
}

void FSimpleAtomic_Runnable::Stop()
{
	UE_LOG(LogTemp, Warning, TEXT("FSimpleAtomic_Runnable::Stop"));
}

void FSimpleAtomic_Runnable::Exit()
{
	UE_LOG(LogTemp, Warning, TEXT("FSimpleAtomic_Runnable::Exit"));
}

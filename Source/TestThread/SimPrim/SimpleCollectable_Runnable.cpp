#include "SimpleCollectable_Runnable.h"


FSimpleCollectable_Runnable::FSimpleCollectable_Runnable(FColor NewColor, ATestThreadGameModeBase* OwnerActor)
{
	Color = NewColor;
	Owner = OwnerActor;
}

FSimpleCollectable_Runnable::~FSimpleCollectable_Runnable()
{
}

uint32 FSimpleCollectable_Runnable::Run()
{
	return 1;
}

void FSimpleCollectable_Runnable::Stop()
{
	
}

void FSimpleCollectable_Runnable::Exit()
{
	
}

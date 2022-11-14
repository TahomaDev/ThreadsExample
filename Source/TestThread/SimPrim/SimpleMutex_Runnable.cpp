#include "SimpleMutex_Runnable.h"

#include <random>

#include "MessageEndpointBuilder.h"

FSimpleMutex_Runnable::FSimpleMutex_Runnable(FColor NewColor, ATestThreadGameModeBase* OwnerActor, bool IsSecondMode)
{
	Color = NewColor;
	Owner = OwnerActor;
	bIsGenerateSecondName = IsSecondMode;

	SenderEndPoint = FMessageEndpoint::Builder("Sender_FSimpleMutes_Runnable").Build();
}

FSimpleMutex_Runnable::~FSimpleMutex_Runnable()
{
}

bool FSimpleMutex_Runnable::Init()
{
	return true;
}

uint32 FSimpleMutex_Runnable::Run()
{
	TArray<FString> VowelLetter{"a", "e", "i", "o", "u", "y"};
	TArray<FString> ConsonantLeter{"s", "d", "w", "r", "t", "g", "h", "n", "m"};

	while(!bIsStopNameGenerator)
	{
		FString Result;

		bool bIsStartWithHISym = GetRandomBool();
		int8 RandomSizeName;
		if (bIsGenerateSecondName)
			RandomSizeName = GetRandomInt8(5,8);
		else
			RandomSizeName = GetRandomInt8(3,5);

		for (int i = 0; i < RandomSizeName; ++i)
		{
			bool FlipFlop = (i%2) ? true : false;

			if (bIsStartWithHISym)
			{
				if (FlipFlop)
					Result.Append(VowelLetter[GetRandomInt8(0, VowelLetter.Num() - 1)]);
				else
					Result.Append(ConsonantLeter[GetRandomInt8(0, ConsonantLeter.Num() - 1)]);
			}
			else
			{
				if (FlipFlop)
					Result.Append(ConsonantLeter[GetRandomInt8(0, ConsonantLeter.Num() - 1)]);
				else
					Result.Append(VowelLetter[GetRandomInt8(0, VowelLetter.Num() - 1)]);

			}
		}

		FPlatformProcess::Sleep(1.8f);
		
		if (bIsGenerateSecondName)
		{
			Owner->SecondName.Enqueue(Result);
		}
		else
		{
			Owner->FirstNameMutex.Lock();
			Owner->FirstNames.Add(Result);
			Owner->FirstNameMutex.Unlock();
		}

		if (SenderEndPoint.IsValid())
		{
			SenderEndPoint->Publish<FBusStructMessage_NameGenerator>(new FBusStructMessage_NameGenerator(bIsGenerateSecondName, Result));
		}
	}
	
	return 1;
}

void FSimpleMutex_Runnable::Stop()
{
	bIsStopNameGenerator = true;
}

void FSimpleMutex_Runnable::Exit()
{
	//IBusMessage
	if (SenderEndPoint.IsValid())
		SenderEndPoint.Reset();
	
	Owner = nullptr;
}

int8 FSimpleMutex_Runnable::GetRandomInt8(int8 min, int8 max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distribution(min,max);
	return distribution(gen);
}

bool FSimpleMutex_Runnable::GetRandomBool()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::bernoulli_distribution d(0.5f);
	return d(gen);
}

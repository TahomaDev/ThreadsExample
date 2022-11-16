// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestThreadGameModeBase.h"
#include "GameFramework/Actor.h"
#include "SRCBoxActor.generated.h"

UCLASS()
class TESTTHREAD_API ASRCBoxActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASRCBoxActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	void InitBP(FInfoNPC NPCInfo);
	void Init(FInfoNPC NPCInfo);
	

};

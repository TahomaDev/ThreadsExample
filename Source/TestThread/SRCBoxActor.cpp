// Fill out your copyright notice in the Description page of Project Settings.


#include "SRCBoxActor.h"

// Sets default values
ASRCBoxActor::ASRCBoxActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASRCBoxActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASRCBoxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASRCBoxActor::Init(FInfoNPC NPCInfo)
{
	InitBP(NPCInfo);
}

void ASRCBoxActor::InitBP_Implementation(FInfoNPC NPCInfo)
{
}


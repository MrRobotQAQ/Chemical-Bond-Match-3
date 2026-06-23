// Fill out your copyright notice in the Description page of Project Settings.

#include "ChemicalBondGameMode.h"

#include "Engine/World.h"
#include "ChemicalBondGameDirector.h"

DEFINE_LOG_CATEGORY(LogChemicalBondGameMode);

AChemicalBondGameMode::AChemicalBondGameMode()
{
	GameDirectorClass = AChemicalBondGameDirector::StaticClass();
}

void AChemicalBondGameMode::BeginPlay()
{
	Super::BeginPlay();

	SpawnGameDirector();
}

void AChemicalBondGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ShutdownGameDirector();

	Super::EndPlay(EndPlayReason);
}

AChemicalBondGameDirector* AChemicalBondGameMode::GetGameDirector() const
{
	return GameDirector;
}

void AChemicalBondGameMode::SpawnGameDirector()
{
	if (GameDirector)
	{
		return;
	}

	if (!GameDirectorClass)
	{
		UE_LOG(LogChemicalBondGameMode, Warning,
			TEXT("[Game:GameMode] Cannot spawn GameDirector because GameDirectorClass is null. GameMode=%s"),
			*GetNameSafe(this));
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogChemicalBondGameMode, Warning,
			TEXT("[Game:GameMode] Cannot spawn GameDirector because World is null. GameMode=%s"),
			*GetNameSafe(this));
		return;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GameDirector = World->SpawnActor<AChemicalBondGameDirector>(GameDirectorClass, FTransform::Identity, SpawnParameters);
	if (!GameDirector)
	{
		UE_LOG(LogChemicalBondGameMode, Warning,
			TEXT("[Game:GameMode] SpawnActor returned null for GameDirectorClass=%s. GameMode=%s"),
			*GetNameSafe(GameDirectorClass.Get()),
			*GetNameSafe(this));
		return;
	}

	GameDirector->InitializeDirector(this);
	GameDirector->StartDirector();
}

void AChemicalBondGameMode::ShutdownGameDirector()
{
	if (!GameDirector)
	{
		return;
	}

	GameDirector->StopDirector();
	GameDirector = nullptr;
}

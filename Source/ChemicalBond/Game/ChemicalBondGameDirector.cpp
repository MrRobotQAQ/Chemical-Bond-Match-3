// Fill out your copyright notice in the Description page of Project Settings.

#include "ChemicalBondGameDirector.h"

#include "ChemicalBondGameMode.h"

DEFINE_LOG_CATEGORY(LogChemicalBondDirector);

AChemicalBondGameDirector::AChemicalBondGameDirector()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AChemicalBondGameDirector::BeginPlay()
{
	Super::BeginPlay();
}

void AChemicalBondGameDirector::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopDirector();

	Super::EndPlay(EndPlayReason);
}

void AChemicalBondGameDirector::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AChemicalBondGameDirector::InitializeDirector(AChemicalBondGameMode* InOwningGameMode)
{
	if (!InOwningGameMode)
	{
		UE_LOG(LogChemicalBondDirector, Warning,
			TEXT("[Game:Director] InitializeDirector failed because OwningGameMode is null. Director=%s"),
			*GetNameSafe(this));
		return;
	}

	OwningGameMode = InOwningGameMode;
}

void AChemicalBondGameDirector::StartDirector()
{
	if (bDirectorStarted)
	{
		return;
	}

	if (!OwningGameMode)
	{
		UE_LOG(LogChemicalBondDirector, Warning,
			TEXT("[Game:Director] StartDirector failed because OwningGameMode is null. Director=%s"),
			*GetNameSafe(this));
		return;
	}

	bDirectorStarted = true;
	SetActorTickEnabled(true);
}

void AChemicalBondGameDirector::StopDirector()
{
	if (!bDirectorStarted)
	{
		return;
	}

	bDirectorStarted = false;
	SetActorTickEnabled(false);
}

bool AChemicalBondGameDirector::IsDirectorStarted() const
{
	return bDirectorStarted;
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ChemicalBondGameInstance.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogChemicalBondGameInstance, Log, All);

// 项目跨关卡生命周期入口，负责后续全局设置、存档入口和关卡切换状态接入。
// 单局玩法状态由 GameMode 和 GameDirector 管理。
UCLASS(Blueprintable)
class CHEMICALBOND_API UChemicalBondGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// 生命周期
	virtual void Init() override;
	virtual void Shutdown() override;
};

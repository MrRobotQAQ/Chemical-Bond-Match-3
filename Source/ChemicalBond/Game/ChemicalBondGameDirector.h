// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChemicalBondGameDirector.generated.h"

class AChemicalBondGameMode;

DECLARE_LOG_CATEGORY_EXTERN(LogChemicalBondDirector, Log, All);

// 单局规则编排入口，负责局内系统的生命周期调度和后续业务系统接入。
// 分子图、刷新、温度、胜负等具体逻辑必须在设计确认后逐项加入。
UCLASS(Blueprintable)
class CHEMICALBOND_API AChemicalBondGameDirector : public AActor
{
	GENERATED_BODY()

public:
	AChemicalBondGameDirector();

	// 生命周期
	virtual void Tick(float DeltaSeconds) override;

	// Director API
	UFUNCTION(BlueprintCallable, Category="ChemicalBond|Director")
	void InitializeDirector(AChemicalBondGameMode* InOwningGameMode);

	UFUNCTION(BlueprintCallable, Category="ChemicalBond|Director")
	void StartDirector();

	UFUNCTION(BlueprintCallable, Category="ChemicalBond|Director")
	void StopDirector();

	UFUNCTION(BlueprintPure, Category="ChemicalBond|Director")
	bool IsDirectorStarted() const;

protected:
	// 生命周期
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	// 运行时状态
	UPROPERTY(Transient)
	TObjectPtr<AChemicalBondGameMode> OwningGameMode = nullptr;

	UPROPERTY(Transient)
	bool bDirectorStarted = false;
};

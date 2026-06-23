#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../AtomTypes.h"
#include "PlaygroundAtomSeeder.generated.h"

class APlaygroundAtom;

// Playground 专用随机粒子播撒器，用于快速搭建测试场粒子分布。
// 它不代表正式场景刷新系统，正式刷新仍由独立设计门禁确认。
UCLASS(Blueprintable)
class CHEMICALBOND_API APlaygroundAtomSeeder : public AActor
{
	GENERATED_BODY()

public:
	APlaygroundAtomSeeder();

protected:
	// 生命周期
	virtual void BeginPlay() override;

	// 策划配置
	// 蓝图配置：Class=PlaygroundAtomSeeder，Range=APlaygroundAtom 派生类，
	// Effect=控制播撒器生成的测试粒子 Actor 类型。
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ChemicalBond|Playground")
	TSubclassOf<APlaygroundAtom> AtomClass;

	// 蓝图配置：Class=PlaygroundAtomSeeder，Range=0..500，
	// Effect=PIE 开始时随机生成的测试粒子数量。
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ChemicalBond|Playground", meta=(ClampMin="0", ClampMax="500"))
	int32 SpawnCount = 42;

	// 蓝图配置：Class=PlaygroundAtomSeeder，Range=0.0..20000.0，
	// Effect=测试粒子围绕播撒器位置随机分布的半径。
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ChemicalBond|Playground", meta=(ClampMin="0.0"))
	float SpawnRadius = 2600.f;

	// 蓝图配置：Class=PlaygroundAtomSeeder，Range=0.0..20000.0，
	// Effect=避免测试粒子生成在玩家起点附近的最小半径。
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ChemicalBond|Playground", meta=(ClampMin="0.0"))
	float InnerClearRadius = 450.f;

	// 蓝图配置：Class=PlaygroundAtomSeeder，Range=任意整数，
	// Effect=固定随机播撒结果，便于比较每次调参后的手感。
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ChemicalBond|Playground")
	int32 RandomSeed = 20260623;

	// 蓝图配置：Class=PlaygroundAtomSeeder，Range=0.0..10000.0，
	// Effect=给测试粒子生成后施加的随机初始冲量强度。
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ChemicalBond|Playground", meta=(ClampMin="0.0"))
	float InitialImpulseStrength = 260.f;

	// 蓝图配置：Class=PlaygroundAtomSeeder，Range=元素枚举数组，
	// Effect=控制测试场会随机出现哪些元素类型。
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ChemicalBond|Playground")
	TArray<EAtomElementType> ElementPool;

private:
	void SpawnAtoms();
	FVector GetRandomSpawnOffset(FRandomStream& RandomStream) const;
	EAtomElementType PickElementType(FRandomStream& RandomStream) const;
};

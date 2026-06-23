#pragma once

#include "CoreMinimal.h"
#include "../Game/ChemicalBondGameMode.h"
#include "PlaygroundGameMode.generated.h"

// Playground 专用 GameMode，复用项目基础 GameMode/GameDirector 生命周期。
// 它只覆盖默认 Pawn，保证点击 PIE 即可测试 C_Player 液体移动手感。
UCLASS(Blueprintable)
class CHEMICALBOND_API APlaygroundGameMode : public AChemicalBondGameMode
{
	GENERATED_BODY()

public:
	APlaygroundGameMode();
};

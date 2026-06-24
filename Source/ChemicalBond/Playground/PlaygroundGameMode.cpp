#include "PlaygroundGameMode.h"

#include "PlaygroundPlayerPawn.h"
#include "UObject/ConstructorHelpers.h"

APlaygroundGameMode::APlaygroundGameMode()
{
	// 优先使用蓝图子类，允许策划在编辑器内调整默认参数。
	// 若蓝图资产不存在（首次 checkout 尚未创建），回退到 C++ 基类保证 PIE 可运行。
	static ConstructorHelpers::FClassFinder<APlaygroundPlayerPawn> PawnBP(
		TEXT("/Game/BP/GameFrame/BP_PlayerAtom"));
	if (PawnBP.Succeeded())
	{
		DefaultPawnClass = PawnBP.Class;
	}
	else
	{
		DefaultPawnClass = APlaygroundPlayerPawn::StaticClass();
	}
}

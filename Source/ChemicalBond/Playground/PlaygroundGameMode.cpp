#include "PlaygroundGameMode.h"

#include "PlaygroundPlayerPawn.h"

APlaygroundGameMode::APlaygroundGameMode()
{
	DefaultPawnClass = APlaygroundPlayerPawn::StaticClass();
}

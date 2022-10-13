#include "multiplayer.h"
#include "Utilities/MinHook.h"

void multiplayer::InitializeMod()
{
	UE4::InitSDK();
	SetupHooks();
}

#include "multiplayer.h"
#include <windows.h>

void CreateMod() { new multiplayer(); }

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	switch ( reason )
	{
		case DLL_PROCESS_ATTACH:
			CreateMod();
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}

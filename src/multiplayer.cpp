#include "multiplayer.h"
#include "Utilities/MinHook.h"

void multiplayer::InitializeMod()
{
	UE4::InitSDK();
	SetupHooks();
	io->socket()->on("message", [&](sio::event &ev) { Log::Print(ev.get_message()->get_string()); });
}

bool init = 0;
void multiplayer::InitGameState() { init = 1; }

void multiplayer::PostBeginPlay(std::wstring ModActorName, UE4::AActor *Actor)
{
	if ( init )
	{
		init = 0;
		Player = UE4::UGameplayStatics::GetPlayerPawn(0);
	}
}

// i'm just going to use this as an event tick because it's called every frame
void multiplayer::DX11Present(ID3D11Device *Device, ID3D11DeviceContext *Ctx, ID3D11RenderTargetView *RTV) {}

// not using this as an event tick because it's only called when the menu is opened
void multiplayer::DrawImGui()
{
	if ( ImGui::Button("send message") )
	{
		io->socket()->emit("message", sio::message::list{"ayup miduck? :)"});
	}
}

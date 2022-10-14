#include "multiplayer.h"
#include "Utilities/MinHook.h"

void multiplayer::InitializeMod()
{
	UE4::InitSDK();
	SetupHooks();
	io->socket()->on("message", [&](sio::event &ev) { messages.push_back(ev.get_message()->get_string()); });
}

// use this variable so that postbeginplay only needs to be called once everytime there's a reload
// just in case a game uses multiple types of player controller
bool init = 0;
void multiplayer::InitGameState() { init = 1; }

void multiplayer::PostBeginPlay(std::wstring ModActorName, UE4::AActor *Actor)
{
	if ( init )
	{
		init = 0;
		player = UE4::UGameplayStatics::GetPlayerPawn(0);
	}
}

// i'm just going to use this as an event tick because it's called every frame
void multiplayer::DX11Present(ID3D11Device *Device, ID3D11DeviceContext *Ctx, ID3D11RenderTargetView *RTV) {}

// not using this as an event tick because it's only called when the menu is opened
void multiplayer::DrawImGui() { connected ? JoinMenu() : ChatMenu(); }

void multiplayer::JoinMenu()
{
	ImGui::Begin("Join Menu", (bool *)1, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::InputText("IP", ip, IM_ARRAYSIZE(ip));
	ImGui::InputText("port", port, IM_ARRAYSIZE(port));
	ImGui::InputText("nickname", nickname, IM_ARRAYSIZE(nickname));
	if ( ImGui::Button("Join") )
	{
		std::string address = "ws://";
		address += ip;
		address += ':';
		address += port;
		Log::Info("attempting to connect to %s", address);
		io->connect(address);
	}
	ImGui::End();
}

void multiplayer::ChatMenu()
{
	ImGui::Begin("Chat Menu", (bool *)1, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::BeginListBox("chat");
	for ( std::string message : messages ) ImGui::Text(message.c_str());
	ImGui::EndListBox();
	if ( ImGui::Button("Leave") ) io->sync_close();
	ImGui::End();
}

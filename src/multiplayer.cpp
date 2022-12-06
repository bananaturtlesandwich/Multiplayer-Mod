#include "multiplayer.h"
#include "Utilities/MinHook.h"

void multiplayer::InitializeMod()
{
	UE4::InitSDK();
	SetupHooks();
	io->socket()->on("message",
	                 [&](sio::event &ev) { messages.push_back(ev.get_message()->get_string().c_str()); });
}

bool init = false;
// so that postbeginplay is only called once everytime there's a reload
// unfortunately player cannot be obtained on initgamestate
void multiplayer::InitGameState() { init = true; }

void multiplayer::PostBeginPlay(std::wstring ModActorName, UE4::AActor *Actor)
{
	if ( init )
	{
		init = false;
		player = UE4::UGameplayStatics::GetPlayerPawn(0);
	}
}

// i'm just going to use this as an event tick because it's called every frame
void multiplayer::DX11Present(ID3D11Device *Device, ID3D11DeviceContext *Ctx, ID3D11RenderTargetView *RTV) {}

// not using this as an event tick because it's only called when the menu is opened
void multiplayer::DrawImGui() { connected ? ChatMenu() : JoinMenu(); }

void multiplayer::JoinMenu()
{
	ImGui::Begin("join", NULL, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::InputText("ip", (char *)&ip, 10);
	ImGui::InputText("port", (char *)&port, 5);
	ImGui::InputText("nickname", (char *)&nickname, 20);
	if ( ImGui::Button("join") )
	{
		// probably not the best way to concatenate :p
		std::string address = "ws://" + ip + ':' + port;
		Log::Info("attempting to connect to %s", address);
		io->connect(address);
	}
	ImGui::End();
}

void multiplayer::ChatMenu()
{
	ImGui::Begin("chat", NULL, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::BeginListBox("");
	for ( const char *message : messages ) ImGui::Text(message);
	ImGui::EndListBox();
	if ( ImGui::Button("leave") )
	{
		io->sync_close();
		connected = false;
	}
	ImGui::End();
}

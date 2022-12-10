#include "multiplayer.h"
#include "Utilities/MinHook.h"

void multiplayer::InitializeMod()
{
	UE4::InitSDK();
	SetupHooks();
	io->socket()->on("message",
	                 [&](sio::event &ev) { messages.push_back(ev.get_message()->get_string().c_str()); });
}

// UE4::UGameplayStatics::GetPlayerController(0) returns null here so do this
bool init;
void multiplayer::InitGameState() { init = true; }

void multiplayer::BeginPlay(UE4::AActor *Actor)
{
	if ( init )
	{
		init = false;
		player = UE4::UGameplayStatics::GetPlayerPawn(0);
		Log::Print("player assigned to " + player->GetName());
	}
}

// i'm just going to use this as an event tick because it's called every frame
void multiplayer::DX11Present(ID3D11Device *dvc, ID3D11DeviceContext *ctx, ID3D11RenderTargetView *rtv) {}

// not using this as an event tick because it's only called when the menu is opened
void multiplayer::DrawImGui() { io->opened() ? ChatMenu() : JoinMenu(); }

void multiplayer::JoinMenu()
{
	ImGui::Begin("join", NULL, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::InputText("ip", (char *)&ip, 10);
	ImGui::InputText("port", (char *)&port, 5);
	ImGui::InputText("nickname", (char *)&nickname, 20);
	if ( ImGui::Button("join") )
	{
		std::string address("ws://" + ip + ':' + port);
		Log::Print("attempting to connect to " + address);
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
	if ( ImGui::InputText("", (char *)&input, 25, ImGuiInputTextFlags_EnterReturnsTrue) )
	{
		io->socket()->emit("message", sio::string_message::create(input));
		input.clear();
	}
	if ( ImGui::Button("leave") ) io->sync_close();
	ImGui::End();
}

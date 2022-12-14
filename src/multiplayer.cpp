#include "multiplayer.h"
#include "Utilities/MinHook.h"

void multiplayer::InitializeMod()
{
	UE4::InitSDK();
	SetupHooks();
	io->socket()->on("message",
	                 [&](sio::event &ev) { messages.push_back(ev.get_message()->get_string().c_str()); });
}

// player does not exist yet and only needs to be retrieved every reload
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
void multiplayer::DX11Present(ID3D11Device *dvc, ID3D11DeviceContext *ctx, ID3D11RenderTargetView *rtv)
{
	if ( io->opened() && player )
	{
		auto trans(player->GetTransform());
		if ( trans.Translation.X )
		{
			assert(trans.Scale3D.Z == ToFTransform(ToMessage(trans)).Scale3D.Z);
			Log::Print("location:%f, %f, %f", trans.Translation.X, trans.Translation.Y, trans.Translation.Z);
		}
	}
}

// can't this as an event tick because it's only called when the menu is open
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
		io->socket()->emit("message", sio::message::list(input));
		input.clear();
	}
	if ( ImGui::Button("leave") ) io->close();
	ImGui::End();
}

sio::message::list multiplayer::ToMessage(UE4::FTransform trans)
{
	sio::message::list list;
	list.push(sio::double_message::create(trans.Translation.X));
	list.push(sio::double_message::create(trans.Translation.Y));
	list.push(sio::double_message::create(trans.Translation.Z));

	list.push(sio::double_message::create(trans.Rotation.W));
	list.push(sio::double_message::create(trans.Rotation.X));
	list.push(sio::double_message::create(trans.Rotation.Y));
	list.push(sio::double_message::create(trans.Rotation.Z));

	list.push(sio::double_message::create(trans.Scale3D.X));
	list.push(sio::double_message::create(trans.Scale3D.Y));
	list.push(sio::double_message::create(trans.Scale3D.Z));
	return list;
}

UE4::FTransform multiplayer::ToFTransform(sio::message::list list)
{
	UE4::FTransform trans;
	trans.Translation = UE4::FVector(list[0]->get_double(), list[1]->get_double(), list[2]->get_double());
	trans.Rotation = UE4::FQuat(list[3]->get_double(), list[4]->get_double(), list[5]->get_double(),
	                            list[6]->get_double());
	trans.Scale3D = UE4::FVector(list[7]->get_double(), list[8]->get_double(), list[9]->get_double());
	return trans;
}

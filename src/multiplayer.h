#pragma once
#include "Mod/Mod.h"
#include "sio_client.h"

class multiplayer : public Mod
{
	public:
	multiplayer()
	{
		ModName = "multiplayer";
		ModVersion = "1.0.0";
		ModDescription = "a universal online multiplayer coremod";
		ModAuthors = "spuds";
		ModLoaderVersion = "2.2.0";

		io.reset(new sio::client());
		ip = "localhost";
		port = "4000";

		ModRef = this;
		CompleteModCreation();
	}

	// Called When Internal Mod Setup is finished
	virtual void InitializeMod() override;

	// InitGameState Call
	virtual void InitGameState() override;

	// BeginPlay of EVERY Actor
	virtual void BeginPlay(UE4::AActor *Actor) override;

	// DX11 hook for when an image will be presented to the screen
	virtual void DX11Present(ID3D11Device *dvc, ID3D11DeviceContext *ctx,
	                         ID3D11RenderTargetView *rtv) override;

	// Call ImGui Here (CALLED EVERY FRAME ON DX HOOK)
	virtual void DrawImGui() override;

	private:
	void JoinMenu();
	void ChatMenu();
	sio::message::list ToMessage(UE4::FTransform trans);
	UE4::FTransform ToFTransform(sio::message::list list);
	std::string ip;
	std::string port;
	std::string nickname;
	std::string input;
	std::vector<const char *> messages;
	std::unique_ptr<sio::client> io;
	UE4::APawn *player;
	std::map<const char *, UE4::APawn *> players;
};

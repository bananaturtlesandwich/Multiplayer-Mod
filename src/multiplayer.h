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
		ModDescription = "A hopefully universal coremod enabling online for most unreal games";
		ModAuthors = "spuds";
		ModLoaderVersion = "2.2.1";
		io.reset(new sio::client());

		ModRef = this;
		CompleteModCreation();
	}

	// Called When Internal Mod Setup is finished
	virtual void InitializeMod() override;

	// InitGameState Call
	virtual void InitGameState() override;

	// PostBeginPlay of EVERY Blueprint ModActor
	virtual void PostBeginPlay(std::wstring ModActorName, UE4::AActor *Actor) override;

	// DX11 hook for when an image will be presented to the screen
	virtual void DX11Present(ID3D11Device *pDevice, ID3D11DeviceContext *pContext,
	                         ID3D11RenderTargetView *pRenderTargetView) override;

	// Call ImGui Here (CALLED EVERY FRAME ON DX HOOK)
	virtual void DrawImGui() override;

	private:
	void JoinMenu();
	void ChatMenu();
	bool connected;
	std::string ip;
	std::string port;
	std::string nickname;
	std::vector<const char *> messages;
	std::unique_ptr<sio::client> io;
	UE4::APawn *player;
	std::vector<UE4::APawn *> players;
};

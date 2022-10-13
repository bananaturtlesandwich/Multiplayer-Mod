#pragma once
#include "Mod/Mod.h"
#include "sio_client.h"
using namespace std;
using namespace sio;

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
		io.reset(new client());

		ModRef = this;
		CompleteModCreation();
	}

	// Called When Internal Mod Setup is finished
	virtual void InitializeMod() override;

	private:
	unique_ptr<client> io;
};

#pragma once
#include "Mod/Mod.h"
#include "sio_client.h"
using namespace std;
using namespace sio;

class socketlib : public Mod
{
	public:
	static unique_ptr<client> io;

	socketlib()
	{
		ModName = "socketlib";
		ModVersion = "1.0.0";
		ModDescription = "A set of UML blueprint bindings for socket.io";
		ModAuthors = "spuds";
		ModLoaderVersion = "2.2.1";
		io.reset(new client());

		ModRef = this;
		CompleteModCreation();
	}

	// Called When Internal Mod Setup is finished
	virtual void InitializeMod() override;
};

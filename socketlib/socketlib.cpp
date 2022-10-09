#include "socketlib.h"
#include "Utilities/MinHook.h"
using namespace UE4;

unique_ptr<client> socketlib::io;

/// <summary>
/// Connects to the remote server
/// </summary>
BPFUNCTION(Connect)
{
	struct Params
	{
		FString ip;
		FString port;
	};
	auto inputs = stack->GetInputParams<Params>();
	string address = "http://";
	address += inputs->ip.ToString();
	address += ':';
	address += inputs->port.ToString();
	Log::Info("Attempting to connect to " + address);
	socketlib::io->connect(address);
}

/// <summary>
/// Disconnects from the remote server
/// </summary>
BPFUNCTION(Disconnect)
{
	socketlib::io->sync_close();
	Log::Info("Disconnected from server");
}

/// <summary>
/// Emits the event given
/// </summary>
BPFUNCTION(SendEvent)
{
	struct Params
	{
		FString event;
	};
	string event = stack->GetInputParams<Params>()->event.ToString();
	socketlib::io->socket()->emit(event);
}

/// <summary>
/// Calls the associated custom event when event of the same name is received - call in PreBeginPlay
/// </summary>
BPFUNCTION(BindEvent)
{
	struct Params
	{
		FString event;
	};
	auto eventname = stack->GetInputParams<Params>()->event;
	socketlib::io->socket()->on(
	    eventname.ToString(), [&](event &ev)
	    { ContextObject->CallFunctionByNameWithArguments(eventname.c_str(), nullptr, NULL, true); });
	Log::Info("bound event %s", eventname.ToString());
}

/// <summary>
/// Emits the event given with a transform and id
/// </summary>
BPFUNCTION(SendTransformEvent)
{
	struct Params
	{
		FString id;
		FString event;
		FTransform transform;
	};
	auto inputs = stack->GetInputParams<Params>();
	auto message = message::list();
	message.push(inputs->id.ToString());
	auto transform = inputs->transform;
	// Translation
	message.push(double_message::create(transform.Translation.X));
	message.push(double_message::create(transform.Translation.Y));
	message.push(double_message::create(transform.Translation.Z));
	// Rotation
	message.push(double_message::create(transform.Rotation.W));
	message.push(double_message::create(transform.Rotation.X));
	message.push(double_message::create(transform.Rotation.Y));
	message.push(double_message::create(transform.Rotation.Z));
	// Scale
	message.push(double_message::create(transform.Scale3D.X));
	message.push(double_message::create(transform.Scale3D.Y));
	message.push(double_message::create(transform.Scale3D.Z));
	socketlib::io->socket()->emit(inputs->event.ToString(), message);
}

/// <summary>
/// Calls the associated transform custom event when event of the same name is received - call in PreBeginPlay
/// </summary>
BPFUNCTION(BindTransformEvent)
{
	struct Params
	{
		FString event;
	};
	auto eventname = stack->GetInputParams<Params>()->event;
	socketlib::io->socket()->on(eventname.ToString(),
	                            [&](event &ev)
	                            {
		                            auto info = ev.get_messages();
		                            FTransform transform;
		                            transform.Translation.X = info.at(1)->get_double();
		                            transform.Translation.Y = info.at(2)->get_double();
		                            transform.Translation.Z = info.at(3)->get_double();

		                            transform.Rotation.W = info.at(4)->get_double();
		                            transform.Rotation.X = info.at(5)->get_double();
		                            transform.Rotation.Y = info.at(6)->get_double();
		                            transform.Rotation.Z = info.at(7)->get_double();

		                            transform.Scale3D.X = info.at(8)->get_double();
		                            transform.Scale3D.Y = info.at(9)->get_double();
		                            transform.Scale3D.Z = info.at(10)->get_double();
		                            struct Params
		                            {
			                            FString id;
			                            FTransform transform;
		                            };
		                            Params params;
		                            auto id = info.at(0)->get_string();
		                            params.id = FString(wstring(id.begin(), id.end()).c_str());
		                            params.transform = transform;
		                            ContextObject->CallFunctionByNameWithArguments(eventname.c_str(), &params,
		                                                                           ContextObject, true);
	                            });
}

/// <summary>
/// A test event for when UML decides to crash the game
/// </summary>
BPFUNCTION(CallEvent)
{
	struct Params
	{
		FString event;
	};
	ContextObject->CallFunctionByNameWithArguments(stack->GetInputParams<Params>()->event.c_str(), nullptr,
	                                               NULL, true);
}

void socketlib::InitializeMod()
{
	InitSDK();
	SetupHooks();
	REGISTER_FUNCTION(Connect);
	REGISTER_FUNCTION(Disconnect);
	REGISTER_FUNCTION(SendEvent);
	REGISTER_FUNCTION(BindEvent);
	REGISTER_FUNCTION(SendTransformEvent);
	REGISTER_FUNCTION(BindTransformEvent);
	REGISTER_FUNCTION(CallEvent);
}

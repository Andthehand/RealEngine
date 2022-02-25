#include <RealEngine.h>
#include <RealEngine/Core/EntryPoint.h>

#include <NetCommon.h>

#include "GameLayer.h"

enum class CustomMsgTypes : uint32_t {
	ServerAccept,
	ServerDeny,
	MessageAll,
	ServerMessage
};

class GameClient : public net::ClientInterface<CustomMsgTypes> {
public:
	void MessageAll() {
		net::Message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::MessageAll;
		Send(msg);
	}
};

class App : public RealEngine::Application {
public:
	App() {
		GameClient client;
		//client.Connect("34.139.21.213", 60000);
		//client.Connect("127.0.0.1", 60000);

		bool key[3] = { false, false, false };
		bool old_key[3] = { false, false, false };

		bool bQuit = false;
		while (!bQuit) {
			if (GetForegroundWindow() == GetConsoleWindow()) {
				key[0] = GetAsyncKeyState('1') & 0x8000;
				key[1] = GetAsyncKeyState('2') & 0x8000;
				key[2] = GetAsyncKeyState('3') & 0x8000;
			}

			if (key[1] && !old_key[1]) client.MessageAll();
			if (key[2] && !old_key[2]) bQuit = true;

			for (int i = 0; i < 3; i++) old_key[i] = key[i];

			if (client.IsConnected()) {
				if (!client.Incoming().empty()) {

					auto msg = client.Incoming().pop_front().msg;

					switch (msg.header.id) {
					case CustomMsgTypes::ServerAccept: {
						// Server has responded to a ping request
						RE_TRACE("Server Accepted Connection");
					}
					break;

					case CustomMsgTypes::ServerMessage: {
						// Server has responded to a ping request	
						uint32_t clientID;
						msg >> clientID;
						RE_TRACE("Hello from [" + std::to_string(clientID) + "]");
					}
					break;
					}
				}
			}
			else {
				RE_TRACE("Server Down");
				bQuit = true;
			}
		}

		PushLayer(new GameLayer);
	}

	~App() {

	}
};

RealEngine::Application* RealEngine::CreateApplication() {
	return new App();
}
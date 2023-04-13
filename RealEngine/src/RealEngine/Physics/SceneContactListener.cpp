#include "repch.h"
#include "SceneContactListener.h"

#include "RealEngine/Scripting/ScriptEngine.h"

namespace RealEngine {
	void SceneContactListener::BeginContact(b2Contact* contact) {
		//Update fixture A
		UUID bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData().pointer;

		if(bodyUserData != 0)
			ScriptEngine::OnBeginContactEntity(bodyUserData);

		//Update fixture B
		bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData().pointer;
		if (bodyUserData != 0)
			ScriptEngine::OnBeginContactEntity(bodyUserData);
	}

	void SceneContactListener::EndContact(b2Contact* contact) {
		//Update fixture A
		UUID bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData().pointer;
		if (bodyUserData != 0)
			ScriptEngine::OnEndContactEntity(bodyUserData);

		//Update fixture B
		bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData().pointer;
		if (bodyUserData != 0)
			ScriptEngine::OnEndContactEntity(bodyUserData);
	}
}

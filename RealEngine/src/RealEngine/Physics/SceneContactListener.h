#include <box2d/b2_world_callbacks.h>
#include <box2d/b2_contact.h>

namespace RealEngine {
	class SceneContactListener : public b2ContactListener {
	public:
		virtual void BeginContact(b2Contact* contact) override;

		virtual void EndContact(b2Contact* contact) override;
	};
}

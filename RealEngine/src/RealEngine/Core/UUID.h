#pragma once
#include <xhash>

namespace RealEngine {
	class UUID {
	public:
		UUID();
		UUID(uint64_t uuid);
	
		operator uint64_t() const{ return m_UUID; }
	private:
		uint64_t m_UUID;
	};
}

namespace std {
	template <typename T> struct hash;

	template<>
	struct hash<RealEngine::UUID> {
		std::size_t operator()(const RealEngine::UUID& uuid) const {
			return (uint64_t)uuid;
		}
	};
}

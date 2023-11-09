#pragma once
#include <string>
#include <vector>

#include "spdlog/fmt/ostr.h"

namespace RealEngine {
	class StringBuilder {
		uint32_t string_length = 0;

		std::vector<std::string> strings;
		std::vector<const char*> c_strings;

		// -1 means it's a Godot String
		// a natural number means C string.
		std::vector<int32_t> appended_strings;

	public:
		StringBuilder& append(const std::string& p_string);
		StringBuilder& append(const char* p_cstring);

		inline StringBuilder& operator+(const std::string& p_string) {
			return append(p_string);
		}

		inline StringBuilder& operator+(const char* p_cstring) {
			return append(p_cstring);
		}

		inline void operator+=(const std::string& p_string) {
			append(p_string);
		}

		inline void operator+=(const char* p_cstring) {
			append(p_cstring);
		}

		template<typename OStream>
		friend OStream& operator<<(OStream& os, const StringBuilder& builder) {
			return os << builder.as_string();
		}

		inline int num_strings_appended() const {
			return (int)appended_strings.size();
		}

		inline uint32_t get_string_length() const {
			return string_length;
		}

		std::string as_string() const;

		inline operator std::string() const {
			return as_string();
		}

		StringBuilder() {}
	};
}

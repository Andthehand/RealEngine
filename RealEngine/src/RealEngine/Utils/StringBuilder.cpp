#include "repch.h"
#include "StringBuilder.h"

#include <new>

namespace RealEngine {
	StringBuilder& StringBuilder::append(const std::string& p_string) {
		if (p_string.empty()) {
			return *this;
		}

		strings.push_back(p_string);
		appended_strings.push_back(-1);

		string_length += (uint32_t)p_string.length();

		return *this;
	}

	StringBuilder& StringBuilder::append(const char* p_cstring) {
		int32_t len = (int32_t)strlen(p_cstring);

		c_strings.push_back(p_cstring);
		appended_strings.push_back(len);

		string_length += len;

		return *this;
	}

	std::string StringBuilder::as_string() const {
		if (string_length == 0) {
			return "";
		}

		char* buffer = new char[string_length];

		int current_position = 0;

		int godot_string_elem = 0;
		int c_string_elem = 0;

		for (int i = 0; i < appended_strings.size(); i++) {
			if (appended_strings[i] == -1) {
				// Godot string
				const std::string& s = strings[godot_string_elem];

				memcpy(buffer + current_position, s.data(), s.length() * sizeof(char));

				current_position += (int)s.length();

				godot_string_elem++;
			}
			else {
				const char* s = c_strings[c_string_elem];

				for (int32_t j = 0; j < appended_strings[i]; j++) {
					buffer[current_position + j] = s[j];
				}

				current_position += appended_strings[i];

				c_string_elem++;
			}
		}

		std::string final_string = std::string(buffer, string_length);

		delete[] buffer;

		return final_string;
	}
}

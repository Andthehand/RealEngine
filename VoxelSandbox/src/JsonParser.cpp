#include "JsonParser.h"
#include <fstream>

#include <json.hpp>
#include <iostream>

using namespace nlohmann;

RealEngine::Ref<RealEngine::Texture2DArray> JsonParser::s_Texture;
std::unordered_map<std::string, BlockData> JsonParser::s_BlockData;

void JsonParser::Init() {
	std::ifstream f("assets/blocks/Blocks.json");
	json data = json::parse(f);

	std::vector<std::string> files;

	uint32_t blockOffset = 0;
	for (auto& [key, value] : data["blocks"].items()) {
		std::vector<std::string> textures = value["textures"].get<std::vector<std::string>>();
		if (textures.size() == 1) {
			files.push_back("assets/blocks/" + textures[0]);
			s_BlockData.insert({ key, BlockData{ blockOffset, blockOffset, blockOffset } });
			blockOffset++;
		}
		else {
			files.push_back("assets/blocks/" + textures[0]);
			files.push_back("assets/blocks/" + textures[1]);
			files.push_back("assets/blocks/" + textures[2]);
			s_BlockData.insert({ key, BlockData{ blockOffset++, blockOffset++, blockOffset++ } });
		}
	}

	s_Texture = RealEngine::Texture2DArray::Create(files.data(), (uint32_t)files.size());
	s_Texture->Bind();
}

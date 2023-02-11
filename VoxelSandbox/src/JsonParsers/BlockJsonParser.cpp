#include "JsonParsers/BlockJsonParser.h"

#include <fstream>
#include <iostream>

#include <json.hpp>

using namespace nlohmann;

void BlockJsonParser::Parse(std::unordered_map<uint32_t, BlockData>& blockData, RealEngine::Ref<RealEngine::Texture2DArray>& texture) {
	std::ifstream f("assets/blocks/Blocks.json");
	json data = json::parse(f);

	std::vector<std::string> files;

	uint32_t blockOffset = 0;
	for (auto& [key, value] : data["blocks"].items()) {
		uint32_t id = value["ID"];
		std::string name = value["name"];

		std::vector<std::string> textures = value["textures"].get<std::vector<std::string>>();
		if (textures.size() == 1) {
			files.push_back("assets/blocks/" + textures[0]);
			blockData.insert({ id, BlockData{ name, blockOffset, blockOffset, blockOffset } });
			blockOffset++;
		}
		else {
			files.push_back("assets/blocks/" + textures[0]);
			files.push_back("assets/blocks/" + textures[1]);
			files.push_back("assets/blocks/" + textures[2]);
			blockData.insert({ id, BlockData{ name, blockOffset++, blockOffset++, blockOffset++ } });
		}
	}

	texture = RealEngine::Texture2DArray::Create(files.data(), (uint32_t)files.size());
	texture->Bind();
}

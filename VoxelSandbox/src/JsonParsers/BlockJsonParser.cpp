#include "JsonParsers/BlockJsonParser.h"

#include <fstream>
#include <iostream>

#include <json.hpp>

#include "Constants.h"

using namespace nlohmann;

void BlockJsonParser::Parse(std::unordered_map<uint32_t, BlockData>& blockData, RealEngine::Ref<RealEngine::Texture2DArray>& texture) {
	std::ifstream f("assets/blocks/Blocks.json");
	json data = json::parse(f);

	RE_ASSERT(data["formatVersion"] == Constants::JSON_FORMAT_VERSION, "JSON format doesn't match current version");

	std::vector<std::string> files;
	uint32_t blockOffset = 0;
	for (auto& [key, value] : data["blocks"].items()) {
		uint32_t id = value["ID"];
		std::string name = value["name"];

		std::vector<std::string> textures = value["textures"].get<std::vector<std::string>>();
		bool insertSuccess = false;
		switch (textures.size()) {
		case 1:
			files.push_back("assets/blocks/" + textures[0]);
			insertSuccess = blockData.insert({ id, BlockData{ name, blockOffset, blockOffset, blockOffset } }).second;
			blockOffset++;
			break;
		case 2:
			files.push_back("assets/blocks/" + textures[0]);
			files.push_back("assets/blocks/" + textures[1]);
			insertSuccess = blockData.insert({ id, BlockData{ name, blockOffset++, blockOffset, blockOffset++ } }).second;
			break;
		case 3:
			files.push_back("assets/blocks/" + textures[0]);
			files.push_back("assets/blocks/" + textures[1]);
			files.push_back("assets/blocks/" + textures[2]);
			insertSuccess = blockData.insert({ id, BlockData{ name, blockOffset++, blockOffset++, blockOffset++ } }).second;
			break;
		}

		RE_ASSERT(insertSuccess, "ID already in blockData");
	}

	texture = RealEngine::Texture2DArray::Create(files.data(), (uint32_t)files.size(), 5);
	texture->Bind();
}

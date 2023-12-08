#include "ShaderPanelSerializer.h"

#include <filesystem>
#include <fstream>

#include <yaml-cpp/yaml.h>

#include "ImNode.h"

namespace RealEngine {	
	void ShaderPanelSerializer::Serialize(Ref<ShaderPanel> shaderPanel, const std::filesystem::path& filepath) {
		RE_PROFILE_FUNCTION();

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Shader Type" << YAML::Value << filepath.stem().string();
		out << YAML::Key << "Nodes" << YAML::Value << YAML::BeginSeq;

		uintptr_t largestID = 0;
		for(const auto& node : shaderPanel->GetNodes()) {
			out << YAML::BeginMap;
			out << YAML::Key << "Name" << YAML::Value << node->GetName();
			out << YAML::Key << "Option Path" << YAML::Value << node->GetOptionPath() + std::string("/") + node->GetName();
			out << YAML::Key << "ID" << YAML::Value << node->ID.Get();

			if (node->ID.Get() > largestID)
				largestID = node->ID.Get();
			out << YAML::EndMap;
		};

		out << YAML::EndSeq;
		out << YAML::Key << "Largest ID" << YAML::Value << largestID;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	Ref<ShaderPanel> ShaderPanelSerializer::Deserialize(const std::filesystem::path& filepath) {
		YAML::Node data;
		try {
			data = YAML::LoadFile(filepath.string());
		}
		catch (YAML::ParserException e) {
			RE_CORE_CRITICAL("{0}: On line {1}", e.msg, e.mark.line);
			RE_CORE_ASSERT(false);
			return nullptr;
		}

		if (!data["Shader Type"]) {
			RE_CORE_ASSERT(false);
			return nullptr;
		}

		Ref<ShaderPanel> shaderPanel = CreateRef<ShaderPanel>(data["Shader Type"].as<std::string>().c_str());
		for (const auto& node : data["Nodes"]) {
			std::string name = node["Name"].as<std::string>();
			std::string optionPath = node["Option Path"].as<std::string>();
			SetNextID(node["ID"].as<int>());
			shaderPanel->AddNodeByRegisterTree(optionPath);
		};
		shaderPanel->BuildNodes();

		SetNextID(data["Largest ID"].as<int>());

		return shaderPanel;
	}
}

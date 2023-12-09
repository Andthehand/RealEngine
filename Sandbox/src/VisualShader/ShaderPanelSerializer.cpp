#include "ShaderPanelSerializer.h"

#include <filesystem>
#include <fstream>

#include <yaml-cpp/yaml.h>

#include "ImNode.h"

namespace RealEngine {	
	void ShaderPanelSerializer::Serialize(Ref<ShaderPanel> shaderPanel[2], const std::filesystem::path& filepath) {
		RE_PROFILE_FUNCTION();

		YAML::Emitter out;

		static std::string shaderType[2] = { "Vertex", "Fragment" };

		uintptr_t largestID = 0;
		for (int i = 0; i < 2; i++) {
			out << YAML::BeginMap;
			out << YAML::Key << "Shader Type" << YAML::Value << shaderType[i];
			out << YAML::Key << "Nodes" << YAML::Value << YAML::BeginSeq;

			for (const auto& node : shaderPanel[i]->GetNodes()) {
				out << YAML::BeginMap;
				out << YAML::Key << "Name" << YAML::Value << node->GetName();
				out << YAML::Key << "Option Path" << YAML::Value << node->GetOptionPath() + std::string("/") + node->GetName();
				out << YAML::Key << "ID" << YAML::Value << node->ID.Get();

				out << YAML::Key << "Links" << YAML::Value << YAML::BeginSeq;
				for (const auto& input : node->Inputs) {
					if (input.ConnectedPin) {
						out << YAML::BeginMap;
						out << YAML::Key << "Input ID" << YAML::Value << input.ID.Get();
						out << YAML::Key << "Output ID" << YAML::Value << input.ConnectedPin->ID.Get();
						out << YAML::EndMap;
					}
				}
				out << YAML::EndSeq;

				if (node->ID.Get() > largestID)
					largestID = node->ID.Get();
				out << YAML::EndMap;
			};

			out << YAML::EndSeq;
			out << YAML::EndMap;
		}

		out << YAML::BeginMap;
		out << YAML::Key << "Largest ID" << YAML::Value << largestID;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	bool ShaderPanelSerializer::Deserialize(const std::filesystem::path& filepath, Ref<ShaderPanel>* shaderPanels) {
		std::vector<YAML::Node> data;
		try {
			data = YAML::LoadAllFromFile(filepath.string());
		}
		catch (YAML::ParserException e) {
			RE_CORE_CRITICAL("{0}: On line {1}", e.msg, e.mark.line);
			RE_CORE_ASSERT(false);
			return false;
		}

		for (int i = 0; i < 2; i++) {
			if (!data[i]["Shader Type"]) {
				RE_CORE_ASSERT(false);
				return false;
			}
			
			shaderPanels[i] = CreateRef<ShaderPanel>(data[i]["Shader Type"].as<std::string>().c_str());
			
			std::vector<int> inputIDs;
			std::vector<int> outputIDs;

			for (const auto& node : data[i]["Nodes"]) {
				std::string name = node["Name"].as<std::string>();
				std::string optionPath = node["Option Path"].as<std::string>();
				SetNextID(node["ID"].as<int>());
				shaderPanels[i]->AddNodeByRegisterTree(optionPath);

				for (const auto& input : node["Links"]) {
					inputIDs.push_back(input["Input ID"].as<int>());
					outputIDs.push_back(input["Output ID"].as<int>());
				}
			};
			shaderPanels[i]->BuildNodes();

			for (int j = 0; j < inputIDs.size(); j++)
				shaderPanels[i]->AddLink(inputIDs[j], outputIDs[j]);
		}
		
		SetNextID(data[2]["Largest ID"].as<int>());
	
	
		return true;
	}
}

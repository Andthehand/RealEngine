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
			out << YAML::BeginMap; //Shader Panel Data
			out << YAML::Key << "Shader Type" << YAML::Value << shaderType[i];
			out << YAML::Key << "Nodes" << YAML::Value << YAML::BeginSeq;

			for (const auto& node : shaderPanel[i]->GetNodes()) {
				out << YAML::BeginMap; //Node Data
				out << YAML::Key << "Name" << YAML::Value << node->GetName();
				out << YAML::Key << "Option Path" << YAML::Value << node->GetOptionPath() + std::string("/") + node->GetName();
				out << YAML::Key << "ID" << YAML::Value << node->ID.Get();

				out << YAML::Key << "Links" << YAML::Value << YAML::BeginSeq;
				for (const auto& input : node->Inputs) {
					if (input.ConnectedPin) {
						out << YAML::BeginMap; //Link Data
						out << YAML::Key << "Input ID" << YAML::Value << input.ID.Get();
						out << YAML::Key << "Output ID" << YAML::Value << input.ConnectedPin->ID.Get();
						out << YAML::EndMap; //Link Data
					}
				}
				out << YAML::EndSeq;

				if (node->ID.Get() > largestID)
					largestID = node->ID.Get();
				out << YAML::EndMap; //Node Data
			};

			out << YAML::EndSeq;
			out << YAML::EndMap; //Shader Panel Data
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
			
			std::vector<int> inputIDs;
			std::vector<int> outputIDs;

			for (int j = 0; j < data[i]["Nodes"].size(); j++) {
				const auto& node = data[i]["Nodes"][j];

				//Add any links connected to the node
				for (const auto& input : node["Links"]) {
					inputIDs.push_back(input["Input ID"].as<int>());
					outputIDs.push_back(input["Output ID"].as<int>());
				}

				//Init the shader panel
				if (j == 0) {
					UniqueId::SetNextID(node["ID"].as<int>());
					shaderPanels[i] = CreateRef<ShaderPanel>(data[i]["Shader Type"].as<std::string>().c_str());
					continue;
				}

				//Add the node itself
				std::string name = node["Name"].as<std::string>();
				std::string optionPath = node["Option Path"].as<std::string>();
				int nextID = node["ID"].as<int>();
				UniqueId::SetNextID(nextID);
				shaderPanels[i]->AddNodeByRegisterTree(optionPath);
			}
			shaderPanels[i]->BuildNodes();

			for (int j = 0; j < inputIDs.size(); j++)
				shaderPanels[i]->AddLink(inputIDs[j], outputIDs[j]);
		}
		
		UniqueId::SetNextID(data[2]["Largest ID"].as<int>());
	
		return true;
	}
}

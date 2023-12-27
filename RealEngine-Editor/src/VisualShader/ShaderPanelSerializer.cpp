#include "ShaderPanelSerializer.h"
#include "ImNode.h"

#include <filesystem>
#include <fstream>

#include <yaml-cpp/yaml.h>

namespace RealEngine {	
	namespace Utils {
		static inline std::string GetVariantTypeName(const Variant& variant) {
			switch (variant.m_Type) {
				case Variant::Type::BOOL:		return "Bool";
				case Variant::Type::INT:		return "Int";
				case Variant::Type::FLOAT:		return "Float";
				case Variant::Type::STRING:		return "String";
				case Variant::Type::VECTOR2:	return "Vector2";
				case Variant::Type::VECTOR2I:	return "Vector2i";
				case Variant::Type::VECTOR3:	return "Vector3";
				case Variant::Type::VECTOR3I:	return "Vector3i";
				case Variant::Type::VECTOR4:	return "Vector4";
				case Variant::Type::VECTOR4I:	return "Vector4i";
				default:
					RE_CORE_ASSERT(false, "Variant type unknown");
					return "Unknown";
			}
		}

		static inline Variant::Type GetVariantTypeFromString(const std::string& type) {
			if (type == "Bool")				return Variant::Type::BOOL;
			else if (type == "Int")			return Variant::Type::INT;
			else if (type == "Float")		return Variant::Type::FLOAT;
			else if (type == "String")		return Variant::Type::STRING;
			else if (type == "Vector2")		return Variant::Type::VECTOR2;
			else if (type == "Vector2i")	return Variant::Type::VECTOR2I;
			else if (type == "Vector3")		return Variant::Type::VECTOR3;
			else if (type == "Vector3i")	return Variant::Type::VECTOR3I;
			else if (type == "Vector4")		return Variant::Type::VECTOR4;
			else if (type == "Vector4i")	return Variant::Type::VECTOR4I;
			
			RE_CORE_ASSERT(false, "Variant type unknown");
			return Variant::Type::NIL;
		}
	}

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
					if (input.IsConnected()) {
						out << YAML::BeginMap; //Link Data
						out << YAML::Key << "Input ID" << YAML::Value << input.ID.Get();
						out << YAML::Key << "Output ID" << YAML::Value << input.ConnectedPin->ID.Get();
						out << YAML::EndMap; //Link Data
					}
				}
				out << YAML::EndSeq;

				//ContentType
				if (!node->Content.empty()) {
					out << YAML::Key << "Content" << YAML::Value;
					out << YAML::Flow << node->GetVariantOptionsIndex();
				}

				int constantIndex = 0;
				out << YAML::Key << "Input Constants" << YAML::Value << YAML::BeginSeq; //Input Constants
				for (Pin& pin : node->Inputs) {
					if (!pin.HasDefaultValue || pin.IsConnected()) {
						constantIndex++;
						continue;
					}

					const Variant& constant = node->GetConstant(constantIndex);
					out << YAML::BeginMap;
					out << YAML::Key << "Type" << YAML::Value << Utils::GetVariantTypeName(constant);
					out << YAML::Key << "Index" << YAML::Value << constantIndex;
					out << YAML::Key << "Value" << YAML::Value;

					//Use C style casts
					switch (constant.m_Type) {
						case Variant::Type::BOOL:
							out << (bool)constant;
							break;
						case Variant::Type::INT:
							out << (int)constant;
							break;
						case Variant::Type::FLOAT:
							out << (float)constant;
							break;
						case Variant::Type::STRING:
							out << (std::string)constant;
							break;
						case Variant::Type::VECTOR2:
							out << (glm::vec2)constant;
							break;
						case Variant::Type::VECTOR2I:
							out << (glm::ivec2)constant;
							break;
						case Variant::Type::VECTOR3:
							out << (glm::vec3)constant;
							break;
						case Variant::Type::VECTOR3I:
							out << (glm::ivec3)constant;
							break;
						case Variant::Type::VECTOR4:
							out << (glm::vec4)constant;
							break;
						case Variant::Type::VECTOR4I:
							out << (glm::ivec4)constant;
							break;
						default:
							RE_CORE_ASSERT(false, "Variant type unknown");
							break;
					}
					out << YAML::EndMap;
					constantIndex++;
				}
				out << YAML::EndSeq; //Input Constants

				out << YAML::Key << "Output Constants" << YAML::Value << YAML::BeginSeq; //Output Constants
				for (Pin& pin : node->Outputs) {
					if (!pin.HasDefaultValue)
						continue;


					const Variant& constant = node->GetConstant(constantIndex);
					out << YAML::BeginMap;
					out << YAML::Key << "Type" << YAML::Value << Utils::GetVariantTypeName(constant);
					out << YAML::Key << "Index" << YAML::Value << constantIndex;
					out << YAML::Key << "Value" << YAML::Value;

					//Use C style casts
					switch (constant.m_Type) {
						case Variant::Type::BOOL:
							out << (bool)constant;
							break;
						case Variant::Type::INT:
							out << (int)constant;
							break;
						case Variant::Type::FLOAT:
							out << (float)constant;
							break;
						case Variant::Type::STRING:
							out << (std::string)constant;
							break;
						case Variant::Type::VECTOR2:
							out << (glm::vec2)constant;
							break;
						case Variant::Type::VECTOR2I:
							out << (glm::ivec2)constant;
							break;
						case Variant::Type::VECTOR3:
							out << (glm::vec3)constant;
							break;
						case Variant::Type::VECTOR3I:
							out << (glm::ivec3)constant;
							break;
						case Variant::Type::VECTOR4:
							out << (glm::vec4)constant;
							break;
						case Variant::Type::VECTOR4I:
							out << (glm::ivec4)constant;
							break;
						default:
							RE_CORE_ASSERT(false, "Variant type unknown");
							break;
					}
					out << YAML::EndMap;
					constantIndex++;
				}
				out << YAML::EndSeq; //Output Constants

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
				Ref<ShaderNode> shaderNode = shaderPanels[i]->AddNodeByRegisterTree(optionPath);

				//Setup the node Content
				if (node["Content"]) {
					for (int k = 0; k < node["Content"].size(); k++) {
						shaderNode->SetVariantOptionsIndex(k, node["Content"][k].as<int>());
					}
				}

				for (auto& constant : node["Input Constants"]) {
					Variant::Type type = Utils::GetVariantTypeFromString(constant["Type"].as<std::string>());
					int index = constant["Index"].as<int>();

					switch (type) {
					case Variant::Type::BOOL:
						shaderNode->SetConstant(index, constant["Value"].as<bool>());
						break;
					case Variant::Type::INT:
						shaderNode->SetConstant(index, constant["Value"].as<int>());
						break;
					case Variant::Type::FLOAT:
						shaderNode->SetConstant(index, constant["Value"].as<float>());
						break;
					case Variant::Type::STRING:
						shaderNode->SetConstant(index, constant["Value"].as<std::string>());
						break;
					case Variant::Type::VECTOR2:
						shaderNode->SetConstant(index, constant["Value"].as<glm::vec2>());
						break;
					case Variant::Type::VECTOR2I:
						shaderNode->SetConstant(index, constant["Value"].as<glm::ivec2>());
						break;
					case Variant::Type::VECTOR3:
						shaderNode->SetConstant(index, constant["Value"].as<glm::vec3>());
						break;
					case Variant::Type::VECTOR3I:
						shaderNode->SetConstant(index, constant["Value"].as<glm::ivec3>());
						break;
					case Variant::Type::VECTOR4:
						shaderNode->SetConstant(index, constant["Value"].as<glm::vec4>());
						break;
					case Variant::Type::VECTOR4I:
						shaderNode->SetConstant(index, constant["Value"].as<glm::ivec4>());
						break;
					default:
						RE_CORE_ASSERT(false, "Variant type unknown");
						break;
					}
				}

				for (auto& constant : node["Output Constants"]) {
					Variant::Type type = Utils::GetVariantTypeFromString(constant["Type"].as<std::string>());
					int index = constant["Index"].as<int>();

					switch (type) {
						case Variant::Type::BOOL:
							shaderNode->SetConstant(index, constant["Value"].as<bool>());
							break;
						case Variant::Type::INT:
							shaderNode->SetConstant(index, constant["Value"].as<int>());
							break;
						case Variant::Type::FLOAT:
							shaderNode->SetConstant(index, constant["Value"].as<float>());
							break;
						case Variant::Type::STRING:
							shaderNode->SetConstant(index, constant["Value"].as<std::string>());
							break;
						case Variant::Type::VECTOR2:
							shaderNode->SetConstant(index, constant["Value"].as<glm::vec2>());
							break;
						case Variant::Type::VECTOR2I:
							shaderNode->SetConstant(index, constant["Value"].as<glm::ivec2>());
							break;
						case Variant::Type::VECTOR3:
							shaderNode->SetConstant(index, constant["Value"].as<glm::vec3>());
							break;
						case Variant::Type::VECTOR3I:
							shaderNode->SetConstant(index, constant["Value"].as<glm::ivec3>());
							break;
						case Variant::Type::VECTOR4:
							shaderNode->SetConstant(index, constant["Value"].as<glm::vec4>());
							break;
						case Variant::Type::VECTOR4I:
							shaderNode->SetConstant(index, constant["Value"].as<glm::ivec4>());
							break;
						default:
							RE_CORE_ASSERT(false, "Variant type unknown");
							break;
					}
				}
			}
			shaderPanels[i]->BuildNodes();

			for (int j = 0; j < inputIDs.size(); j++)
				shaderPanels[i]->AddLink(inputIDs[j], outputIDs[j]);
		}
		
		//Set the largest ID + 10 offset so no ID's are the same
		UniqueId::SetNextID(data[2]["Largest ID"].as<int>() + 10);
	
		return true;
	}
}

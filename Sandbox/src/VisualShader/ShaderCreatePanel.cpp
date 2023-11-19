#include "ShaderCreatePanel.h"

//TODO: This might be really bad
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

#include "Nodes/ShaderNodes.h"

namespace RealEngine {
	ShaderCreatePanel::ShaderCreatePanel() {
		ImNode::Config config;
		config.SettingsFile = "Simple.json";
		m_Context = ImNode::CreateEditor(&config);

		m_HeaderBackground = RealEngine::Texture2D::Create("Resources/Icons/ShaderCreate/BlueprintHeader.png");

		//Init Testing Nodes
		m_Nodes.emplace_back(CreateRef<FragmentShaderOutputNode>());

		m_Nodes.emplace_back(CreateRef<ShaderConstantVec4Node>());

		BuildNodes();

		m_QueuedCompile = true;
	}

	static const char* PinTypeToString(PinType pinType) {
		switch (pinType) {
		case PinType::Bool:			return "bool ";
		case PinType::Int:			return "int ";
		case PinType::Float:		return "float ";
		case PinType::Vector2:		return "vec2 ";
		case PinType::Vector3:		return "vec3 ";
		case PinType::Vector4:		return "vec4 ";
		case PinType::Sampler2D:	return "sampler2D ";
		}
		RE_CORE_ASSERT(false);
		return nullptr;
	}

	ImColor ShaderCreatePanel::GetIconColor(PinType type) {
		switch (type) {
			case PinType::Bool:			return ImColor(220, 48, 48);
			case PinType::Int:			return ImColor(68, 201, 156);
			case PinType::Float:		return ImColor(97, 217, 245);
			case PinType::Vector2:		return ImColor(189, 145, 242);
			case PinType::Vector3:		return ImColor(214, 125, 237);
			case PinType::Vector4:		return ImColor(255, 32, 242);
			case PinType::Sampler2D:	return ImColor(255, 255, 0);
		}

		//RE_CORE_ASSERT(false);
		return ImColor(255, 255, 255);
	};

	void ShaderCreatePanel::DrawPinIcon(const Pin& pin, bool connected, int alpha) {
		//TODO: Implement!
		IconType iconType;
		ImColor  color = GetIconColor(pin.Type);
		color.Value.w = alpha / 255.0f;
		switch (pin.Type) {
			case PinType::Bool:     
				iconType = IconType::Diamond; break;
			case PinType::Int:      
			case PinType::Float:    
			case PinType::Vector2:  
			case PinType::Vector3:  
			case PinType::Vector4:  
				iconType = IconType::Circle; break;
			case PinType::Sampler2D:
				iconType = IconType::RoundSquare; break;
			default:
				RE_CORE_ASSERT(false);
				return;
		}

		//ax::Widgets::Icon(iconType, connected, color, ImColor(32, 32, 32, alpha));
		ax::Widgets::Icon(iconType, connected, color, ImColor(32, 32, 32, alpha));
	};

	void ShaderCreatePanel::OnImGuiRender() {
		static bool p_open = true;
		ImGui::Begin("Shader Creation Editor", &p_open, ImGuiWindowFlags_MenuBar);

		//Mostly differnt gui option menus
		{
			if (ImGui::BeginMenuBar()) {
				if (ImGui::BeginMenu("Options")) {
					if (ImGui::MenuItem("Compile"))
						m_QueuedCompile = true;
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			ImNode::SetCurrentEditor(m_Context);
			ImNode::Begin("My Editor");

			//You have to suspend ImNode because https://github.com/thedmd/imgui-node-editor/issues/37#issuecomment-549122793
			ImNode::Suspend();
			if (ImNode::ShowBackgroundContextMenu()) {
				ImGui::OpenPopup("Create New Node");
			}

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
			if (ImGui::BeginPopup("Create New Node")) {

				if (ImGui::MenuItem("Compile")) m_QueuedCompile = true;
				ImGui::EndPopup();
			}
			ImGui::PopStyleVar();
			ImNode::Resume();
		}

		//Rendering of Nodes and lines
		{
			ImVec2 cursorTopLeft = ImGui::GetCursorScreenPos();

			ImNode::Utilities::BlueprintNodeBuilder builder((ImTextureID)(uint64_t)m_HeaderBackground->GetRendererID(),
				m_HeaderBackground->GetWidth(), m_HeaderBackground->GetHeight());

			for (Ref<ShaderNode> node : m_Nodes) {

				//TODO: Implement
				/*if (node.Type != NodeType::Blueprint && node.Type != NodeType::Simple)
					continue;*/

				//const bool isSimple = node.Type == NodeType::Simple;
				const bool isSimple = false;

				builder.Begin(node->ID);
				if (!isSimple) {
					//TODO: Implement
					//builder.Header(node.Color);
					builder.Header();
					ImGui::Spring(0);
					ImGui::TextUnformatted(node->Name.c_str());
					ImGui::Spring(1);
					ImGui::Dummy(ImVec2(0, 28));
					ImGui::Spring(0);
					builder.EndHeader();
				}

				for (Pin& input : node->Inputs) {
					float alpha = ImGui::GetStyle().Alpha;
					//TODO: Implement!
					/*if (newLinkPin && !CanCreateLink(newLinkPin, &input) && &input != newLinkPin)
						alpha = alpha * (48.0f / 255.0f);*/

					builder.Input(input.ID);
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
					//TODO: Implement!
					DrawPinIcon(input, IsPinLinked(input.ID), (int)(alpha * 255));
					ImGui::Spring(0);
					if (!input.Name.empty()) {
						static glm::vec4 test = glm::vec4(1.0f);
						ImNode::DrawNodeVec4Control(input.Name, test);
						ImGui::Spring(0);
						ImGui::TextUnformatted(input.Name.c_str());
						ImGui::Spring(0);
					}
					ImGui::PopStyleVar();
					builder.EndInput();
				}

				for (Pin& output : node->Outputs) {
					float alpha = ImGui::GetStyle().Alpha;
					//TODO: Implement!
					/*if (newLinkPin && !CanCreateLink(newLinkPin, &output) && &output != newLinkPin)
						alpha = alpha * (48.0f / 255.0f);*/

					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
					builder.Output(output.ID);
					if (!output.Name.empty()) {
						ImGui::Spring(0);
						ImGui::TextUnformatted(output.Name.c_str());
					}
					ImGui::Spring(0);
					DrawPinIcon(output, IsPinLinked(output.ID), (int)(alpha * 255));
					ImGui::PopStyleVar();
					builder.EndOutput();
				}

				builder.End();
			}
		}

		// Submit Links for drawing
		for (Link& linkInfo : m_Links)
			ImNode::Link(linkInfo.Id, linkInfo.InputPin->ID, linkInfo.OutputPin->ID);

		HandleInteraction();

		// End of interaction with editor.
		ImNode::End();

		ImNode::SetCurrentEditor(nullptr);

		ImGui::End();
	}
	
	void ShaderCreatePanel::HandleInteraction() {
		// Handle creation action, returns true if editor want to create new object (node or link)
		if (ImNode::BeginCreate()) {
			//Helper Lambda
			//TODO: Move to its own function?
			auto showLabel = [](const char* label, ImColor color) {
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetTextLineHeight());
				ImVec2 size = ImGui::CalcTextSize(label);

				ImVec2 padding = ImGui::GetStyle().FramePadding;
				ImVec2 spacing = ImGui::GetStyle().ItemSpacing;

				ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(spacing.x, -spacing.y));

				ImVec2 rectMin = ImGui::GetCursorScreenPos() - padding;
				ImVec2 rectMax = ImGui::GetCursorScreenPos() + size + padding;

				ImDrawList* drawList = ImGui::GetWindowDrawList();
				drawList->AddRectFilled(rectMin, rectMax, color, size.y * 0.15f);
				ImGui::TextUnformatted(label);
			};


			ImNode::PinId inputPinId, outputPinId;
			if (ImNode::QueryNewLink(&inputPinId, &outputPinId) 
				// Check that both IDs are valid
				&& (inputPinId && outputPinId)) {

				Pin* inputPin = FindPin(inputPinId);
				Pin* outputPin = FindPin(outputPinId);
				
				//Make sure the input and output are actually in the correct orientation
				if (inputPin->Kind == PinKind::Output) {
					std::swap(inputPin, outputPin);
					std::swap(inputPinId, outputPinId);
				}

				if (inputPin == outputPin) {
					ImNode::RejectNewItem();
				}
				else if (inputPin->Node == outputPin->Node) {
					//Can't connect input and output from same node together
					showLabel("Pin Cannot Connect to Same Node", ImColor(171, 44, 44, 180));
					ImNode::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				else if (inputPin->Kind == outputPin->Kind) {
					//Can't connect 2 inputs or 2 outputs together
					std::string kind = inputPin->Kind == PinKind::Input ? "Inputs" : "Outputs";

					showLabel(("Can't Connect 2 " + kind + " Together").c_str(), ImColor(171, 44, 44, 180));
					ImNode::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				else if (inputPin->Type != outputPin->Type) {
					//Can't connect 2 pins of different types together
					showLabel("Can't Connect Pins of Different Types", ImColor(171, 44, 44, 180));
					ImNode::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				else if (IsPinLinked(inputPinId)) {
					//Inputs can't have 2 connections
					showLabel("Pin Already Connected", ImColor(171, 44, 44, 180));
					ImNode::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				else {
					//If hovering over valid pin
					showLabel("Connect Pairs", ImColor(44, 171, 44, 180));

					//If mouse released basically
					if (ImNode::AcceptNewItem(ImColor(0, 255, 0), 2.0f)) {
						//Just add the link to the list
						m_Links.push_back({ ImNode::LinkId(m_NextLinkId++), inputPin, outputPin });

						inputPin->ConnectedPin = outputPin;
						outputPin->ConnectedPin = inputPin;
					}
				}
			}
		}
		ImNode::EndCreate(); // Wraps up object creation action handling.


		// Handle deletion action
		if (ImNode::BeginDelete()) {
			// There may be many links marked for deletion, let's loop over them.
			ImNode::LinkId deletedLinkId;
			while (ImNode::QueryDeletedLink(&deletedLinkId)) {
				// If you agree that link can be deleted, accept deletion.
				if (!ImNode::AcceptDeletedItem())
					continue;

				// Then remove link from your data.
				for (Link& link : m_Links) {
					if (link.Id != deletedLinkId)
						continue;

					//This is so scuffed
					//It is now less scuffed
					//and finally now it's way less scuffed
					link.InputPin->ConnectedPin = nullptr;
					link.OutputPin->ConnectedPin = nullptr;

					m_Links.erase(&link);
				}
			}
		}
		ImNode::EndDelete();
	}

	void ShaderCreatePanel::OnUpdate() {
		if (m_QueuedCompile) {
			Compile();
			m_QueuedCompile = false;
		}
	
	}

	void ShaderCreatePanel::RecursiveSearch(const ShaderNode* currentNode, StringBuilder& shaderCode, StringBuilder& globalCode, std::unordered_set<uint64_t>* nodeTracking) {

		//Going down the chain of Nodes until it reaches the end
		for (const Pin& inputPin: currentNode->Inputs) {
			//If the Node isn't connected continue
			if (inputPin.ConnectedPin == nullptr)
				continue;

			//If connected node hasn't been visited yet then visit it
			const uint64_t nextNodeID = (uint64_t)inputPin.ConnectedPin->Node->ID;
			if(nodeTracking->find(nextNodeID) == nodeTracking->end()) {
				nodeTracking->insert(nextNodeID);
				RecursiveSearch(inputPin.ConnectedPin->Node, shaderCode, globalCode, nodeTracking);
			}

		}

		//Reserve space for the input and output variables
		std::vector<std::string> inputVars;
		inputVars.resize(currentNode->Inputs.size());
		std::string* inputs = inputVars.data();

		std::vector<std::string> outputVars;
		outputVars.resize(currentNode->Outputs.size());
		std::string* outputs = outputVars.data();

		for (int i = 0; i < currentNode->Outputs.size(); i++) {
			if(currentNode->Outputs[i].ConnectedPin == nullptr)
				continue;

			std::string varName;
			varName = "out_";
			varName += currentNode->Name;
			varName += "_";
			varName += currentNode->Outputs[i].Name;
			varName.erase(remove_if(varName.begin(), varName.end(), isspace), varName.end());

			outputs[i] = PinTypeToString(currentNode->Outputs[i].Type) + varName;
		}

		//Add to the shader code
		for(int i = 0; i < currentNode->Inputs.size(); i++) {
			const Pin* connectedPin = currentNode->Inputs[i].ConnectedPin;

			if(connectedPin == nullptr)
				continue;

			inputs[i] = "out_";
			inputs[i] += connectedPin->Node->Name;
			inputs[i] += "_";
			inputs[i] += connectedPin->Name;
			inputs[i].erase(remove_if(inputs[i].begin(), inputs[i].end(), isspace), inputs[i].end());
		}

		shaderCode += currentNode->GenerateCode(outputs, inputs);
		globalCode += currentNode->GenerateGlobalCode(inputs);
	}

	void ShaderCreatePanel::Compile() {
		StringBuilder fragShaderCode;
		StringBuilder fragGlobalCode;
		fragGlobalCode += "\n#version 450 core\n\n";
		fragShaderCode += "void main() { \n";

		std::unordered_set<uint64_t> nodeTracking;

		//m_Nodes[0] is always the output node
		RecursiveSearch(m_Nodes[0].get(), fragShaderCode, fragGlobalCode, &nodeTracking);

		fragShaderCode += "}";
		fragGlobalCode += "\n";

		RE_CORE_WARN("{0}", fragGlobalCode.as_string() + fragShaderCode.as_string());

		StringBuilder vertShaderCode;
		StringBuilder vertGlobalCode;
		vertShaderCode += "#version 450 core\n"
			"in vec3 aPos;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos, 1.0);\n"
			"}";

		m_PreviewShader = Shader::Create("Preview Shader", vertGlobalCode.as_string() + vertShaderCode.as_string(), fragGlobalCode.as_string() + fragShaderCode.as_string());
	}

	Pin* ShaderCreatePanel::FindPin(ImNode::PinId id) {
		for (Ref<ShaderNode>& node : m_Nodes) {
			for (Pin& pin : node->Inputs)
				if (pin.ID == id)
					return &pin;

			for (Pin& pin : node->Outputs)
				if (pin.ID == id)
					return &pin;
		}

		RE_CORE_CRITICAL("Coundn't find Pin");
		return nullptr;
	}

	Link* ShaderCreatePanel::FindPinLink(ImNode::PinId id) {
		for (Link& link : m_Links)
			if (link.InputPin->ID == id || link.OutputPin->ID == id)
				return &link;

		return nullptr;
	}

	bool ShaderCreatePanel::IsPinLinked(ImNode::PinId id) {
		for (Link& link : m_Links)
			if (link.InputPin->ID == id || link.OutputPin->ID == id)
				return true;

		return false;
	}

	void ShaderCreatePanel::BuildNodes() {
		for (Ref<ShaderNode> node : m_Nodes)
			node->BuildNode();
	}
}

﻿# pragma once
# include <imgui_node_editor.h>

namespace ax::NodeEditor::Utilities {

struct BlueprintNodeBuilder {
    BlueprintNodeBuilder(ImTextureID texture = nullptr, int textureWidth = 0, int textureHeight = 0);

    void Begin(NodeId id);
    void End();

    void Header(const ImVec4& color = ImVec4(1, 1, 1, 1));
    void EndHeader();

	void BypassEndHeader();

    void Input(PinId id);
    void EndInput();

    void Output(PinId id);
    void EndOutput();
private:
	enum class Stage {
		Invalid,
		Begin,
		Header,
		Content,
		Input,
		Output,
		End
	};

	bool SetStage(Stage stage);

    void Pin(PinId id, ax::NodeEditor::PinKind kind);
    void EndPin();

    ImTextureID HeaderTextureId;
    int         HeaderTextureWidth;
    int         HeaderTextureHeight;
    NodeId      CurrentNodeId;
    Stage       CurrentStage;
    ImU32       HeaderColor;
    ImVec2      NodeMin;
    ImVec2      NodeMax;
    ImVec2      HeaderMin;
    ImVec2      HeaderMax;
    ImVec2      ContentMin;
    ImVec2      ContentMax;
    bool        HasHeader;
};
} // namespace ax::NodeEditor::Utilities

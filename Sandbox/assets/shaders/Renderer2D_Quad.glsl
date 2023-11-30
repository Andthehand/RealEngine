// Basic Texture Shader

#type vertex
#version 450 core

in vec3 a_Position;
in vec4 a_Color;
in vec2 a_TexCoord;
in uint a_TexIndex;
in float a_TilingFactor;
in int a_EntityID;

uniform Camera {
	mat4 u_ViewProjection;
};

struct VertexOutput {
	vec4 Color;
	vec2 TexCoord;
	float TilingFactor;
};

out VertexOutput Output;
out flat uint v_TexIndex;
out flat int v_EntityID;

void main() {
	Output.Color = a_Color;
	Output.TexCoord = a_TexCoord;
	Output.TilingFactor = a_TilingFactor;
	v_TexIndex = a_TexIndex;
	v_EntityID = a_EntityID;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

out vec4 o_Color;
out int o_EntityID;

struct VertexOutput {
	vec4 Color;
	vec2 TexCoord;
	float TilingFactor;
};

in VertexOutput Input;
in flat uint v_TexIndex;
in flat int v_EntityID;

uniform sampler2D u_Textures[32];

void main() {
	vec4 texColor = Input.Color;
	
	texColor *= texture(u_Textures[v_TexIndex], Input.TexCoord * Input.TilingFactor);
	
	if (texColor.a == 0.0)
		discard;

	o_Color = texColor;
	o_EntityID = v_EntityID;
}

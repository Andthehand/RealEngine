#type vertex
#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in uint a_Data;

uniform samplerBuffer u_TexCoordTexture;
uniform mat4 u_ViewProjection;

out vec2 v_TexCoord;

#define UV_ID_BITMASK			uint(0x00000003)
#define TEX_COORD_ID_BITMASK	uint(0x00000FFC)
#define WIDTH_OFFSET_BITMASK	uint(0x000FF000)
#define HEIGHT_OFFSET_BITMASK	uint(0x0FF00000)

vec2 GetTextureCoord() {
	uint textureCoordId = (a_Data & TEX_COORD_ID_BITMASK) >> 2;
	uint UV = a_Data & UV_ID_BITMASK;

	//textureCoordId should always step by 4
	int index = int((textureCoordId * uint(4)) + UV);
	return vec2(texelFetch(u_TexCoordTexture, index).rg);
}

vec2 GetOffset() {
	uint XOffset = (a_Data & WIDTH_OFFSET_BITMASK)  >> 12;
	uint YOffset = (a_Data & HEIGHT_OFFSET_BITMASK) >> 20;

	return vec2(XOffset, YOffset);
}

void main() {
	v_TexCoord = GetTextureCoord() * GetOffset();
	
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 450

uniform sampler2D ourTexture;

in vec2 v_TexCoord;

out vec4 FragColor;

void main() {
	FragColor = texture(ourTexture, v_TexCoord);
	//FragColor = vec4(v_TexCoord.x, v_TexCoord.y, 0.0f, 1.0f);
}

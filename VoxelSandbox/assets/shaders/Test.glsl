#type vertex
#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in uint a_Data;

out vec2 v_TexCoords;

uniform samplerBuffer u_TexCoordTexture;

uniform mat4 u_ViewProjection;

#define Tex_Coord_ID_BITMASK uint(0x000000FF)
#define UV_ID_BITMASK uint(0x00000300)

vec2 GetTextureCoords() {
	uint textureCoordId = a_Data & Tex_Coord_ID_BITMASK;
	uint UVId = (a_Data & UV_ID_BITMASK) >> 8;

	int index = int(textureCoordId + UVId);
	return vec2(texelFetch(u_TexCoordTexture, index).rg);
}

void main() {
	v_TexCoords = GetTextureCoords();
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 450

in vec2 v_TexCoords;


out vec4 FragColor;

void main() {
	//FragColor = vec4(150, 150, 1.0f, 1.0f);
	FragColor = vec4(v_TexCoords.x * 150, v_TexCoords.y * 150, 0.0f, 1.0f);
}

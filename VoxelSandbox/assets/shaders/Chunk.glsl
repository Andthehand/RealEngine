#type vertex
#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in uint a_Data;

uniform samplerBuffer u_TexCoordTexture;
uniform mat4 u_ViewProjection;

out vec2 v_TexCoord;

#define Tex_Coord_ID_BITMASK uint(0x000000FF)
#define UV_ID_BITMASK uint(0x00000300)

vec2 GetTextureCoords() {
	uint textureCoordId = a_Data & Tex_Coord_ID_BITMASK;
	uint UV = (a_Data & UV_ID_BITMASK) >> 8;

	//textureCoordId should always step by 4
	int index = int((textureCoordId * uint(4)) + UV);
	return vec2(texelFetch(u_TexCoordTexture, index).rg);
}

void main() {
	v_TexCoord = GetTextureCoords();
	
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

#type vertex
#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in uint a_Data;

uniform samplerBuffer u_TexCoordTexture;
uniform mat4 u_ViewProjection;

out vec3 v_TexCoord;

#define UV_ID_BITMASK			uint(0x00000003)
#define SIDE_ID_BITMASK			uint(0x0000000C)
#define TEX_COORD_ID_BITMASK	uint(0x00003FF0)
#define WIDTH_OFFSET_BITMASK	uint(0x003FC000)
#define HEIGHT_OFFSET_BITMASK	uint(0x3FC00000)

const vec2 coord[2][4] = vec2[][](
	vec2[](vec2(0.0f, 0.0f), vec2(1.0f, 0.0f), vec2(1.0f, 1.0f), vec2(0.0f, 1.0f)),
	vec2[](vec2(0.0f, 1.0f), vec2(0.0f, 0.0f), vec2(1.0f, 0.0f), vec2(1.0f, 1.0f)));

vec3 GetTextureCoord() {
	int textureCoordId = int((a_Data & TEX_COORD_ID_BITMASK) >> 4);
	uint UV = a_Data & UV_ID_BITMASK;
	uint Side = (a_Data & SIDE_ID_BITMASK) >> 2;

	return vec3(coord[Side][UV], texelFetch(u_TexCoordTexture, textureCoordId).r);
}

vec2 GetOffset() {
	uint XOffset = (a_Data & WIDTH_OFFSET_BITMASK)  >> 14;
	uint YOffset = (a_Data & HEIGHT_OFFSET_BITMASK) >> 22;

	return vec2(XOffset, YOffset);
}

void main() {
	vec3 tempCoord = GetTextureCoord();
	v_TexCoord = vec3(tempCoord.xy * GetOffset(), tempCoord.z);
	
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 450

uniform sampler2DArray ourTexture;

in vec3 v_TexCoord;

out vec4 FragColor;

void main() {
			  //texture(textures, vec3(UV, layer))
	FragColor = texture(ourTexture, v_TexCoord);
	//FragColor = vec4(v_TexCoord.x, v_TexCoord.y, 0.0f, 1.0f);
}

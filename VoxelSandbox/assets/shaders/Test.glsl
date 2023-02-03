#type vertex
#version 450

layout(location = 0) in vec3 a_Vert;
layout(location = 1) in vec3 a_Position;

uniform mat4 u_ViewProjection;

void main() {
	gl_Position = u_ViewProjection * vec4((a_Vert * (gl_InstanceID / 5.0)) + a_Position, 1.0);
}

#type fragment
#version 450

uniform samplerBuffer u_TexCoordTexture;

out vec4 FragColor;

void main() {
	FragColor = vec4(texelFetch(u_TexCoordTexture, 0).r * 150, texelFetch(u_TexCoordTexture, 1).r * 150, 1.0f, 1.0f);
}

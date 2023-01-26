#type vertex
#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;

out vec2 v_TexCoord;

void main() {
	v_TexCoord = a_TexCoord;
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

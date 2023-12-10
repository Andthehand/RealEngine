Vertex:
#version 450 core

in vec3 a_Position;
in vec2 a_UV;

#ifndef IMPLEMENTUV
out vec2 v_UV;
#endif

void main() { 
	v_UV = a_UV;
	gl_Position = vec4(a_Position, 1.0);
}

Fragment:
#version 450 core

uniform sampler2D u_7_Texture2D_Sampler2D;
in vec2 v_UV;
out vec4 o_Color;

void main() { 
	vec4 out_7_Texture2D_Color = texture(u_7_Texture2D_Sampler2D, v_UV);
	o_Color = out_7_Texture2D_Color;
}

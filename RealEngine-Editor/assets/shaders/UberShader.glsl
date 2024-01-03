#type vertex
#version 450 core

#CustomDefines

in vec3 a_Position;
in vec2 a_UV;

#ifdef IMPLEMENTUV
out vec2 v_UV;
#endif

//Custom Globals
#GlobalCustomCode

void main() {
	#ifndef IMPLEMENTUV
	v_UV = a_UV;
	#endif

#CustomCode
}

#type fragment
#version 450 core

#CustomDefines

#ifdef IMPLEMENTUV
in vec2 v_UV;
#endif
out vec4 o_Color;
out vec3 o_Normal;

//Custom Globals
#GlobalCustomCode

void main() {
#CustomCode
}

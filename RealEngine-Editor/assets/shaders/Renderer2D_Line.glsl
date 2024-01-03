#type vertex
#version 450 core

in vec3 a_Position;
in vec4 a_Color;
in int a_EntityID;

uniform Camera {
	mat4 u_ViewProjection;
};

struct VertexOutput {
	vec4 Color;
};

out VertexOutput Output;
out flat int v_EntityID;

void main() {
	Output.Color = a_Color;
	v_EntityID = a_EntityID;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

out vec4 o_Color;
out int o_EntityID;

struct VertexOutput {
	vec4 Color;
};

in VertexOutput Input;
in flat int v_EntityID;

void main() {
	o_Color = Input.Color;
	o_EntityID = v_EntityID;
}

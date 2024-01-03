#type vertex
#version 450 core

in vec3 a_WorldPosition;
in vec3 a_LocalPosition;
in vec4 a_Color;
in float a_Thickness;
in float a_Fade;
in int a_EntityID;

uniform Camera {
	mat4 u_ViewProjection;
};

struct VertexOutput {
	vec3 LocalPosition;
	vec4 Color;
	float Thickness;
	float Fade;
};

out VertexOutput Output;
out flat int v_EntityID;

void main() {
	Output.LocalPosition = a_LocalPosition;
	Output.Color = a_Color;
	Output.Thickness = a_Thickness;
	Output.Fade = a_Fade;

	v_EntityID = a_EntityID;

	gl_Position = u_ViewProjection * vec4(a_WorldPosition, 1.0);
}

#type fragment
#version 450 core

out vec4 o_Color;
out int o_EntityID;

struct VertexOutput {
	vec3 LocalPosition;
	vec4 Color;
	float Thickness;
	float Fade;
};

in VertexOutput Input;
in flat int v_EntityID;

void main() {
    // Calculate distance and fill circle with white
    float distance = 1.0 - length(Input.LocalPosition);
    float circle = smoothstep(0.0, Input.Fade, distance);
    circle *= smoothstep(Input.Thickness + Input.Fade, Input.Thickness, distance);

	if (circle == 0.0)
		discard;

    // Set output color
    o_Color = Input.Color;
	o_Color.a *= circle;

	o_EntityID = v_EntityID;
}

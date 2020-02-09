#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 barycentric;
layout (location = 2) in vec3 normal;

out vec3 vertColor;
out vec3 vertBarycentric;

uniform bool showWireframe;

uniform vec3 light;
uniform vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);

	float diffuse = max(dot(vec3(model * vec4(normal, 0)), -light), 0.0);
	vertColor = (color * 0.5) + (diffuse * 0.5);

	vertBarycentric = barycentric;
}

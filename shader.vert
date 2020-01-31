#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 block_color;

out vec3 color;

uniform vec3 light;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	float diffuse = max(dot(normal, light), 0.0);
	gl_Position = projection * view * model * vec4(position, 1.0);
	color = block_color * (diffuse * 0.5 + 0.5);
}
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 barycentric;
layout (location = 2) in vec3 normal;
// layout (location = 3) in vec3 vertexColor;

out vec3 color;

uniform vec3 u_light;
uniform vec3 u_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);

	float diffuse = max(dot(vec3(model * vec4(normal, 0)), -u_light), 0.0);
	color = (u_color * 0.5) + (diffuse * 0.5);

	// vec3 direction = vec3(view[0][2], view[1][2], view[2][2]);
	// if (dot(direction, normal) < 0.0) {
	// 	color = vec3(1.0, 0.0, 0.0);
	// } else {
	// 	color = vec3(1.0, 1.0, 1.0);
	// }
}
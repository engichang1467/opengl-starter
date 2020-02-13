#version 330 core

#extension GL_OES_standard_derivatives : enable

in vec3 vertColor;
in vec3 vertBarycentric;

out vec4 fragColor;

uniform bool showWireframe;

// anit-aliasing
float edgeFactor()
{
    vec3 d = fwidth(vertBarycentric);
    vec3 a3 = smoothstep(vec3(0.0), d*0.95, vertBarycentric);
    return min(min(a3.x, a3.y), a3.z);
}

void main()
{
	if (showWireframe) {
		fragColor = vec4(mix(vec3(0.0), vertColor, edgeFactor()), 1.0);
	} else {
		fragColor = vec4(vertColor, 1.0);
	}
}
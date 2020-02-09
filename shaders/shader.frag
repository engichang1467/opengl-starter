#version 330 core

out vec4 FragColor;

in vec3 color;
in vec3 bary;

uniform bool uShowWireframe;


float thickness = 0.025;

#extension GL_OES_standard_derivatives : enable
float edgeFactor(){
    vec3 d = fwidth(bary);
    vec3 a3 = smoothstep(vec3(0.0), d*0.95, bary);
    return min(min(a3.x, a3.y), a3.z);
}


void main()
{
	// if (bary.x < thickness || bary.y < thickness || bary.z < thickness) {
	// 	FragColor = vec4(0.0, 0.0, 0.0, (1.0-edgeFactor())*0.95);
	// } else {
	// 	FragColor = vec4(0.0, 0.0, 0.0, (1.0-edgeFactor())*0.95);
	// }
	// if(gl_FrontFacing) {
	//     FragColor = vec4(0.0, 0.0, 0.0, (1.0-edgeFactor())*0.95);
	// } else {
	//     FragColor = vec4(0.0, 0.0, 0.0, (1.0-edgeFactor())*0.7);
	// }

    // FragColor.rgb = mix(vec3(0.0), vec3(0.5), edgeFactor());
	// FragColor = vec4(0.0, 0.0, 0.0, (1.0-edgeFactor())*0.95);

	if (uShowWireframe) {
		FragColor = vec4(mix(vec3(0.0), color, edgeFactor()), 1.0);
	} else {
		FragColor = vec4(color, 1.0);
	}


}
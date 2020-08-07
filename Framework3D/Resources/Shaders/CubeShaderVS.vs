uniform mat4 u_wvp;

uniform float u_fogStart;
uniform float u_fogLength;

attribute vec3 a_posL;
varying vec3 v_pos;

varying float v_fogDensity;

void main()
{
	vec4 position = u_wvp*vec4(a_posL, 1.0);
	gl_Position = position;
	
	float distanceToCam = sqrt(position.x*position.x+position.y*position.y+position.z*position.z);
	v_fogDensity = clamp((distanceToCam-u_fogStart)/u_fogLength,0.0,1.0);

	v_pos = a_posL;
}
   
uniform mat4 u_wvp;
//uniform sampler2D u_heightmap;
//uniform float u_heightmapScaleFactor;

uniform float u_fogStart;
uniform float u_fogLength;

attribute vec3 a_posL;
attribute vec2 a_uv;
varying vec2 v_uv;

varying float v_fogDensity;

void main()
{
	//vec4 h_texel = texture2D(u_heightmap,a_uv);
	//float h = 1.0 - ((0.3 * h_texel.r) + (0.59 * h_texel.g) + (0.11 * h_texel.b));
	//gl_Position = u_wvp*vec4(a_posL.x, a_posL.y + h * u_heightmapScaleFactor, a_posL.z, 1.0);

	vec4 position = u_wvp*vec4(a_posL, 1.0);
	gl_Position = position;

	float distanceToCam = sqrt(position.x*position.x+position.y*position.y+position.z*position.z);
	//float distanceToCam = position.z;
	v_fogDensity = clamp((distanceToCam-u_fogStart)/u_fogLength,0.0,1.0);

	v_uv = a_uv;
}
   
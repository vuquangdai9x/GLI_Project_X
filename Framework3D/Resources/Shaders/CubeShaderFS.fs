precision mediump float;

uniform samplerCube u_mainTexture;
uniform vec4 u_fogColor;

varying vec3 v_pos;
varying float v_fogDensity;

void main(void)
{
	vec4 color = textureCube(u_mainTexture, v_pos);
	gl_FragColor = color + v_fogDensity / 2.0 * (u_fogColor - color);
	//gl_FragColor = color;
}
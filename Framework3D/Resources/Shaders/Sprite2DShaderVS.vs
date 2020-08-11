uniform vec2 u_size;
uniform vec2 u_offset;
uniform mat4 u_wvp;
uniform float u_texSizeRatio;

attribute vec3 a_posL;
attribute vec2 a_uv;

varying vec2 v_uv;

void main()
{
	gl_Position = u_wvp*vec4(a_posL.x * u_texSizeRatio, a_posL.y, a_posL.z, 1.0);
	v_uv = a_uv * u_size + u_offset;
}
   
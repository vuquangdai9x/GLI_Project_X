attribute float a_angle;
attribute float a_radiusOffset;
attribute float a_radiusMul;
attribute float a_sizeOffset;
attribute vec4 a_colorOffset;

uniform mat4 u_wvp;
uniform float u_radius;
uniform float u_size;

varying vec4 v_colorOffset;

void main()
{
	float radius = a_radiusOffset + u_radius * a_radiusMul;
    float x = radius * cos(a_angle);
    float y = radius * sin(a_angle);
    
    gl_Position = u_wvp * vec4(x, y, 0.0, 1.0);
    gl_PointSize = a_sizeOffset + u_size;

	v_colorOffset = a_colorOffset;
}
   
attribute vec2 a_initPos;
attribute vec2 a_velocityOffset;
attribute float a_velocityMultiplier;
attribute float a_timeOffset;
attribute float a_lifeTime;
attribute float a_size;
attribute vec4 a_colorOffset;

uniform mat4 u_wvp;
uniform float u_time;
uniform vec2 u_velocity;

varying vec4 v_colorOffset;

void main()
{ 
	vec2 pos = a_initPos + (u_velocity+a_velocityOffset)*a_velocityMultiplier*mod((u_time-a_timeOffset),a_lifeTime);
    gl_Position = u_wvp * vec4(pos.x, pos.y, 0.0, 1.0);
    gl_PointSize = a_size;

	v_colorOffset = a_colorOffset;
}
   
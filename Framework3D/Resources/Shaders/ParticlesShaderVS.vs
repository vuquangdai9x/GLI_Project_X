attribute float a_theta;

uniform mat4 u_wvp;
uniform float u_k;
uniform float u_pointSize;

void main(void)
{
    float x = u_k * cos(a_theta);
    float y = u_k * sin(a_theta);
    
    gl_Position = u_wvp * vec4(x, y, 0.0, 1.0);
    gl_PointSize = u_pointSize;
}
   
precision mediump float;
uniform sampler2D u_mainTexture;
varying vec2 v_uv;

vec3 rgb2hsv(vec3 c)
{
    vec4 K=vec4(0.,-1./3.,2./3.,-1.),
    p=mix(vec4(c.bg,K.wz),vec4(c.gb,K.xy),step(c.b,c.g)),
    q=mix(vec4(p.xyw,c.r),vec4(c.r,p.yzx),step(p.x,c.r));
    float d=q.x-min(q.w,q.y),e=1.0e-10;
    return vec3(abs(q.z+(q.w-q.y)/(6.*d+e)),d/(q.x+e),q.x);
}
vec3 hsv2rgb(vec3 c){return c.z*mix(vec3(1),sat(abs(fract(vec3(.1,2./3.,1./3.)+c.x)*6.-3.)-1.),c.y);}
vec3 rgb2hsv(float r,float g,float b){return rgb2hsv(vec3(r,g,b));}
vec3 hsv2rgb(float h,float s,float v){return hsv2rgb(vec3(h,s,v));}
vec3 colorFromKelvin(float temperature) // photographic temperature values are between 15 to 150
{
    float r, g, b;
    if(temperature <= 66.0)
    {
        r = 1.0;
        g = sat((99.4708025861 * log(temperature) - 161.1195681661) / 255.0);
        if(temperature < 19.0)
            b = 0.0;
        else
            b = sat((138.5177312231 * log(temperature - 10.0) - 305.0447927307) / 255.0);
    }
    else
    {
        r = sat((329.698727446 / 255.0) * pow(temperature - 60.0, -0.1332047592));
        g = sat((288.1221695283  / 255.0) * pow(temperature - 60.0, -0.0755148492));
        b = 1.0;
    }
    return vec3(r, g, b);
}
float Luma(vec3 color) { return dot(color, vec3(0.2126, 0.7152, 0.0722)); }
vec3 ACESFilm( vec3 x )
{
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return sat((x*(a*x+b))/(x*(c*x+d)+e));
}
vec3 LinearToSRGB(vec3 rgb)
{
    rgb=max(rgb,vec3(0.,0.,0.));
    return max(1.055*pow(rgb,vec3(0.416666667))-0.055,0.0);
}


vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
  return mod289(((x*34.0)+1.0)*x);
}

float snoise(vec2 v)
  {
  const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                      0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                     -0.577350269189626,  // -1.0 + 2.0 * C.x
                      0.024390243902439); // 1.0 / 41.0
// First corner
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);

// Other corners
  vec2 i1;
  //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
  //i1.y = 1.0 - i1.x;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;

// Permutations
  i = mod289(i); // Avoid truncation effects in permutation
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
		+ i.x + vec3(0.0, i1.x, 1.0 ));

  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;

// Gradients: 41 points uniformly over a line, mapped onto a diamond.
// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;

// Normalise gradients implicitly by scaling m
// Approximation of: m *= inversesqrt( a0*a0 + h*h );
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

// Compute final noise value at P
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}

const vec2 lightCenter = vec2(1.5,1.5);
const vec4 lightColor = vec4(1.0,1.0,0.0,1.0);

void main()
{
	//vec2 deltaPos = v_uv-lightCenter;
	//float lightIntensify = snoise(acos(deltaPos/length(deltaPos)) * 10.0);
	
	//lightIntensify = smoothstep(0.5,1.0,lightIntensify);

	//vec4 color = texture2D(u_mainTexture,v_uv) + lightColor * lightIntensify * 0.5;
	vec4 color = texture2D(u_mainTexture,v_uv);
	//gl_FragColor = vec4(1.0-color.x, 1.0-color.y, 1.0-color.z, 1.0);
	gl_FragColor = color;
}

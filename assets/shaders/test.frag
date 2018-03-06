#version 120

//take in position and normal
varying vec3 color;
varying vec3 fragPos;
varying vec3 normal;
varying vec2 tCoord;

uniform float inpSeed;
uniform float radius;
uniform mat4 roll;
uniform vec3 color1;
uniform vec3 color2;
uniform vec3 color3;
uniform vec3 color4;
uniform vec3 atmosphere;
uniform vec3 horizon;
uniform vec3 lightPos;
uniform vec3 center;
uniform vec2 warp;

uniform vec3 childPos;
uniform float childRad;

//function definitions
float rand(vec2 c);
float noise(vec2 p, float freq );
float pNoise(vec2 p, int res, float seed, float freq);
float normalDistr(vec3 N, vec3 H, float roughness);
float geometrySchlick(float NV, float roughness);
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
float perlin(vec2 domain, int resolution, float seed, float amplitude, float frequency, float weight);
float raySphereIntersect(float radius, vec3 camPos);
float raySphereIntersect(float radius, vec3 camPos, vec3 pCenter);
float distort(vec2 domain, float seed, out vec2 first, out vec2 second);
vec2 fbm(vec2 domain, float seed, float amplitude, float frequency);
vec3 fresnel(float cTheta, vec3 F0);

#define PI 3.1415926

void main(void)
{	
	//generate terrain
	vec2 testColor;
	vec2 testColor2;
	vec4 domainMod = roll*vec4(tCoord.x, tCoord.y, 0, 1);
	vec3 perlinOutput = vec3(distort(domainMod.xy*warp, inpSeed, testColor, testColor2));
	float chooser = pNoise(testColor, 20, 3, 8);
	float chooser2 = pNoise(testColor2, 20, 3, 8);

	vec3 colorMix = mix(color1, color2, chooser*6);
	vec3 colorMix2 = mix(color3, color4, chooser2*6);

	vec3 finalColor = perlinOutput*(colorMix+colorMix2);


	/*
	 * LIGHTING: calculate final lighting for planet
	 */

	//TODO: make lightColor passable by shader
	vec3 lightColor = vec3(7.0f, 6.0f, 5.0f);

	//calculate lighting vectors
	vec3 lightDir = lightPos; //directional light, all comes from same angle
	vec3 viewPos = vec3(0,0,-70.0); //constant, since the camera doesn't move
	vec3 viewDir = viewPos - fragPos;

    vec3 N = normalize(normal);
    vec3 L = normalize(lightDir);
    vec3 V = normalize(viewDir);
    vec3 R = normalize(L+V); //implements blinn-phong
	
    //begin lighting calculations
    float metallic = (chooser2 + 0.3) * warp.x; //disables metallic if theres warp
    float roughness = 1 - metallic;

    //radiance calcuations
    vec3 F0 = vec3(0.04); //dielectric constant
    F0 = mix(F0, finalColor, metallic);
    vec3 F = fresnel(max(dot(N, V), 0.0), F0);

    float nDistribution = normalDistr(N, R, roughness);
	float geometry = geometrySmith(N, V, L, roughness);

    //Cook-Torrance BDRF
    vec3 top = nDistribution * geometry * F;
    float bottom = 4* max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
    vec3 specular = top / bottom;

    vec3 finalSpec = max(F, 0.0);
    vec3 finalDiff = vec3(1.0) - finalSpec;
    finalDiff *= 1.0 - metallic;

    float lightAngle = max(dot(N, L), 0.0);

    //calculate child planet shadow
    float childDiff = 1.0f;

    //check if intersects child planet
    float connect = raySphereIntersect(childRad, lightPos*100.0f, childPos);

    //will darken if a collision occurs
    childDiff = 1 - clamp(pow(connect/(childRad*2.0), 2.0),0,1);

    //negate if currently rendering child
    if (childRad == radius)
    {
    	childDiff = 1.0f;
    }


    //calculate final diffuse component
    lightAngle *= clamp(childDiff,0,1);

    vec3 finalLight = (finalDiff * finalColor / 3.1415 + specular) * lightColor * lightAngle;

    //atmosphere effects
	//calculate atmosphere scattering using ray sphere intersection
    float atmHeight = raySphereIntersect(radius+0.05, viewPos);
    float surfHeight = raySphereIntersect(radius, viewPos);

    //get atmosphere thickness from difference
	float influence = atmHeight - surfHeight;

	//get final atmosphere component
	float power =  clamp(influence,0,1) * clamp(lightAngle, 0 ,1) * 5;
	power  = max(power, 0.0);

	//calculate atmosphere color
	float atmLight = raySphereIntersect(radius+0.05, lightPos*100.0f);
	float surfLight = raySphereIntersect(radius, lightPos*100.0f);
	float lighting = atmLight - surfLight;
	//adjust color influence
	vec3 atmColor = mix(atmosphere, horizon, pow(lighting, 0.3));
	vec3 atmOut = power * atmColor;
	//clamp
	atmOut = max(atmOut, 0.0);

	//perform screen blend
	vec3 final = 1 - (1 - finalLight) * (1 - atmOut);
	gl_FragColor = vec4(final, 1.0f);
}

float raySphereIntersect(float radius, vec3 camPos)
{
	return raySphereIntersect(radius, camPos, center);
}

float raySphereIntersect(float radius, vec3 camPos, vec3 pCenter)
{
	vec3 rayA = camPos - fragPos; //get ray from camera to pixel
	vec3 rayB = camPos - pCenter; //get ray from camera to center L

	//get cos theta
	float theta = dot(normalize(rayA), normalize(rayB));
	//project b (length of radius) onto a
	float tc = length(rayB)*theta;
	float L = length(rayB);

	//get orthographic side between tca and rayB
	float d = sqrt(L*L - tc*tc);

	//get final contribution from radius length and orhtographic length
	float t1c = sqrt(radius*radius - d*d);

	return t1c;
}

//basic noise and perlin functions based on code by Patricio Gonzalez Vivo

//generates random noise from a vec2 input seed
float rand(vec2 seed)
{
	return fract(sin(dot(seed ,vec2(12.9898,78.233))) * 129039.5453);
}

//generate resolution independent noise
float noise(vec2 p, float freq )
{
    //generate units for each vector point
	float unit = 1/freq;
	vec2 ij = floor(p/unit);
	vec2 xy = mod(p,unit)/unit;

	xy = 0.5*(1.-cos(PI*xy));
	float a = rand((ij+vec2(0.0,0.0)));
	float b = rand((ij+vec2(1.0,0.0)));
	float c = rand((ij+vec2(0.0,1.0)));
	float d = rand((ij+vec2(1.0,1.0)));
	float x1 = mix(a, b, xy.x);
	float x2 = mix(c, d, xy.x);
	return mix(x1, x2, xy.y);
}

//layer noise of varying frequencies and amplitudes to generate perlin noise
float pNoise(vec2 p, int res, float seed, float freq)
{

    //initialize values
    float persistance = 0.5;
	float n = 0.0;
	float normK = 0.0;
	float amp = 1.0;
    float f = freq;
	for (int i = 0; i<50; i++){
        //continually increase frequency and decrease amplitude for each iteration
		n += amp * noise(p + seed, f);
		f *= 2.0;
		normK += amp;
		amp *= persistance;

        //break if we hit target amount of iterations
		if (i == res)
            break;
	}

    //normalize output
	float nf = n/normK;
	return nf*nf*nf*nf;
}

//generate final perlin noise
float perlin(vec2 domain, int resolution, float seed, float amplitude, float frequency, float weight)
{
    float pVal = pNoise(domain, resolution, seed, frequency);
    pVal += weight;
    pVal *= amplitude;
    return clamp(pVal, 0.0, 1.0);
}

//algorithm based on Inigo Quilez's article on domain distortion

//this is a helper function in calculating a fractional brownian motion
//this produces the distorted domain for the function to use
vec2 fbm(vec2 domain, float seed, float amplitude, float frequency)
{
    float perlin1 = perlin(domain, 20, seed+24.33, amplitude, frequency, 0);
    float perlin2 = perlin(domain, 20, seed, amplitude, frequency, 0);

    //compact into vec2 and add random offset
    vec2 final = vec2(perlin1 + seed, perlin2 + rand(vec2(seed)));

    //return final fbm
    return final;
}

//does the final distortion
float distort(vec2 domain, float seed, out vec2 first, out vec2 second)
{
    float freq = 4.0;
    float amp = 1.0;

    //base distort
    first = fbm(domain, seed, amp, freq);
    second = fbm(domain + 4.0*first, seed+2.0232, amp *= 0.5, freq += 2.0);

    //collapse to perlin output
    return perlin(domain + 4.0*second, 20, seed, 0.8, 10, 0.3);
}


/*
 * Area for PBR lighting related functions
 */

float normalDistr(vec3 N, vec3 H, float roughness)
{
	// implement normal distribution function
	// (a^2)/(PI((N*H)^2(a^2-1)+1)^2)
	// where a is roughness, N is normal, and H is halfway vec

	float rough2 = pow(roughness, 2);
	float NH = max(dot(N, H), 0.0);
	float NH2 = pow(NH, 2);

	float denominator = (NH2 * (rough2 - 1.0) + 1.0);
	denominator = 3.1415 * pow(denominator, 2);

	return rough2 / denominator;
}

float geometrySchlick(float NV, float roughness)
{
	//implements Schlick GGX
	float r = (roughness + 1.0);
	float k = (r*r) / 8.0;
	return NV / (NV * (1.0 - k) + k);
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	//implements Smith's method
	float NV = max(dot(N, V), 0.0);
	float NL = max(dot(N, L), 0.0);
	float ggx1 = geometrySchlick(NV, roughness);
	float ggx2 = geometrySchlick(NL, roughness);

	return ggx1 * ggx2;
}


vec3 fresnel(float cTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cTheta, 5.0);

}


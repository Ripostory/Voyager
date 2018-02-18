#version 330

in vec2 texCoordModel;
uniform int screenWidth;

out vec4 frag_color;

//function definitions
float rand(vec2 c);
float noise(vec2 p, float freq );
float noise(vec2 p, float freq, float stretch );
float pNoise(vec2 p, int res, float seed, float freq);
float perlin(vec2 domain, int resolution, float seed, float amplitude, float frequency, float weight);
vec2 fbm(vec2 domain, float seed, float amplitude, float frequency);

float distort(vec2 domain, float seed, out vec2 first, out vec2 second);

#define PI 3.1415926

void main(void)
{
   vec2 testColor;
   vec2 testColor2;
   vec3 perlinOutput = vec3(distort(texCoordModel, 55, testColor, testColor2));
   vec3 color1 = vec3(0.8, 0.5, 0.1);
   vec3 color2 = vec3(0.4, 0.9, 0.7);
   vec3 color3 = vec3(0, 0.4, 0.5);
   vec3 color4 = vec3(1, 1, 1);
   float chooser = pNoise(testColor, 20, 3, 8);
   float chooser2 = pNoise(testColor2, 20, 3, 8);
   
   vec3 colorMix = mix(color1, color2, chooser*6);
   vec3 colorMix2 = mix(color3, color4, chooser2*6);
   
   frag_color = vec4(perlinOutput*(colorMix+colorMix2),1);
}

//basic noise and perlin functions based on code by Patricio Gonzalez Vivo

//generates random noise from a vec2 input seed
float rand(vec2 seed){
	return fract(sin(dot(seed ,vec2(12.9898,78.233))) * 129039.5453);
}

//generate resolution independent noise
float noise(vec2 p, float freq ){
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
float pNoise(vec2 p, int res, float seed, float freq){
	
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

float distort(vec2 domain, float seed, out vec2 first, out vec2 second)
{
    float freq = 4.0;
    float amp = 1.0;
    float persistance = 0.5;
    
    //base distort
    first = fbm(domain, seed, amp, freq);
    second = fbm(domain + 4.0*first, seed+2.0232, amp *= 0.5, freq += 2.0);
   
    //collapse to perlin output
    return perlin(domain + 4.0*second, 20, seed, 0.8, 10, 0.3);
}

/*
    for (int i = 0; i < 2; i++)
    {
        //decrease amplitude and increase frequency
        amp *= persistance;
        freq *= 2.0;
        
        //reseed seed
        seed = rand(vec2(1.321+seed, 9.544+seed));
        
        //perform fbm iteration
        output = fbm(domain + 4.0*output, seed, amp, freq);
    }

*/





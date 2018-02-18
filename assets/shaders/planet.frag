#version 330

//take in position and normal
smooth in vec3 color;
in vec3 fragPos;
in vec3 normal;
in vec3 viewDir;
in vec2 texCoordMod;

uniform sampler2D texture;
uniform sampler2D normalMap;

out vec4 frag_color;

in vec3 lightPos;

void main(void)
{	
	//temporary light color
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

	//get norm and light direction;
	vec3 lightDir = lightPos - fragPos;
	//get diffuse strength
	float diff = dot(normal, lightDir) / (length(lightDir) * length(normal));
	diff = clamp(diff, 0, 1);
	
	vec3 diffuse = diff * lightColor;
	
	
	//atmosphere effects
	vec3 atmColor = vec3(0.3f, 0.5f, 0.7f);
	vec3 atmHorizon = vec3(0.8f, 0.2f, 0.1f);
	float horizonBrightness = max(dot(normal, lightDir), 0)/5;
	vec3 atmFinal = mix(atmHorizon, atmColor, pow(diff, 2.0f));
	float atmBrightness = pow(sin(acos(dot(normalize(normal), normalize(viewDir)))), 20.0f)/50;
	atmBrightness = clamp(atmBrightness, 0 ,1);

	vec4 atmOut = vec4(atmFinal * atmBrightness * horizonBrightness , 1.0f);
	vec4 base = (texture2D(texture, texCoordMod) * vec4(diffuse, 1.0));
	frag_color =  texture2D(texture, texCoordMod);
    frag_color = base + atmOut;
}
#version 330 core

struct Material {

    vec3 ambient;
    sampler2D diffuseMap;
    vec3 specular;
    float shininess;
};

struct Light {

	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Lamp {

	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float exponent;
};

struct Flashlight {

	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float exponent;

	float cosInnerCone;
	float cosOuterCone;
	bool on;
};
  
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform Light light;
uniform Material material;
uniform Lamp lamp;
uniform Flashlight lat;
uniform vec3 viewPos;

out vec4 frag_color;

vec3 calcSpotLight();

void main() { 

    // Ambient -------------------------------------------------------------------------
    vec3 ambientLi = light.ambient * material.ambient * vec3(texture(material.diffuseMap, TexCoord));
    vec3 ambientLa = lamp.ambient * material.ambient * vec3(texture(material.diffuseMap, TexCoord));
  	
    // Diffuse -------------------------------------------------------------------------
    vec3 normal = normalize(Normal); 
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 lampDir = normalize(lamp.position - FragPos);
    float NdotLi = max(dot(normal, lightDir), 0.0);
    float NdotLa = max(dot(normal, lampDir), 0.0);
    vec3 diffuseLi = light.diffuse * NdotLi * vec3(texture(material.diffuseMap, TexCoord));
    vec3 diffuseLa = lamp.diffuse * NdotLa * vec3(texture(material.diffuseMap, TexCoord));
    
    // Specular - Blinn-Phong ----------------------------------------------------------
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfDirLi = normalize(lightDir + viewDir);
	vec3 halfDirLa = normalize(lampDir + viewDir);
	float NDotHLi = max(dot(normal, halfDirLi), 0.0);
	float NDotHLa = max(dot(normal, halfDirLa), 0.0);
	vec3 specularLi = light.specular * material.specular * pow(NDotHLi, material.shininess);
	vec3 specularLa = lamp.specular * material.specular * pow(NDotHLa, material.shininess);
	
	// w przypadku lampy dodatkowo trzeba uwzglednic
	float d = length(lamp.position - FragPos);
	float attenuation = 1.0f / (lamp.constant + lamp.linear * d + lamp.exponent * (d * d));

	diffuseLa *= attenuation;
	specularLa *= attenuation;

	// blendujemy swiatlo ? :D co to benidze co to bendzie w blendzie :D
	vec3 ambient = (ambientLi + ambientLa);
	vec3 diffuse = (diffuseLi + diffuseLa);
	vec3 specular = (specularLi + specularLa);


/*	// takie ograniczonko przed przepelnieniem
	if ( ambient.x > 1.0 ) ambient.x = 1.0;
	if ( ambient.y > 1.0 ) ambient.y = 1.0;
	if ( ambient.z > 1.0 ) ambient.z = 1.0;	

	if ( diffuse.x > 1.0 ) diffuse.x = 1.0;
	if ( diffuse.y > 1.0 ) diffuse.y = 1.0;	
	if ( diffuse.z > 1.0 ) diffuse.z = 1.0;

	if ( specular.x > 1.0 ) specular.x = 1.0;
	if ( specular.y > 1.0 ) specular.y = 1.0;
	if ( specular.z > 1.0 ) specular.z = 1.0;
*/

	if ( lat.on ) 
		frag_color = vec4(ambient + diffuse + specular, 1.0) + vec4(calcSpotLight(), 1.0);
	else 
		frag_color = vec4(ambient + diffuse + specular, 1.0);


}

vec3 calcSpotLight()
{
	vec3 lightDir = normalize(lat.position - FragPos);
	vec3 spotDir  = normalize(lat.direction);

	float cosDir = dot(-lightDir, spotDir);  // angle between the lights direction vector and spotlights direction vector
	float spotIntensity = smoothstep(lat.cosOuterCone, lat.cosInnerCone, cosDir);

	// Diffuse ------------------------------------------------------------------------- 
    vec3 normal = normalize(Normal);  
    float NdotL = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = lat.diffuse * NdotL * vec3(texture(material.diffuseMap, TexCoord));
    
     // Specular - Blinn-Phong ----------------------------------------------------------
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfDir = normalize(lightDir + viewDir);
	float NDotH = max(dot(normal, halfDir), 0.0f);
	vec3 specular = lat.specular * material.specular * pow(NDotH, material.shininess);

	// Attenuation using Kc, Kl, Kq -----------------------------------------------------
	float d = length(lat.position - FragPos);  // distance to light
	float attenuation = 1.0f / (lat.constant + lat.linear * d + lat.exponent * (d * d));

	diffuse *= attenuation * spotIntensity;
	specular *= attenuation * spotIntensity;
	
	return (diffuse + specular);
}
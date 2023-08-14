#version 330 core

// light structure 
struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

out vec4 fragColor;

// in's 
in vec2 textureCoordinates;
in vec3 Normal; 
in vec3 fragmentPosition;

// unifomrms 
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

// uniform SpotLight spotlight;
#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirLight dirLight;

uniform vec3 viewPosition;
uniform float shininess;

// function prototypes 
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection);

void main() {
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPosition - fragmentPosition);

    // phase 1: Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // phase 2: Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, fragmentPosition, viewDir);    
    // phase 3: Spot light
    //result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    
    // fragColor = vec4(result, 1.0);
	fragColor = texture(texture_diffuse1, textureCoordinates);
}

// function declarations
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(texture_diffuse1, textureCoordinates));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, textureCoordinates));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, textureCoordinates));

    return (ambient + diffuse + specular);
} 

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection){
	// ambient lighting for the point light
    vec3 ambient = texture(texture_diffuse1, textureCoordinates).rgb * light.ambient;

    // diffuse lighting for the point light
    vec3 lightDirection = normalize(light.position - fragmentPosition);
    float diff = max(dot(normal, lightDirection), 0.0); // Clamp the dot product between 0 and 1
    vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, textureCoordinates).rgb;

    // specular lighting for the point light
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(reflectDirection, viewDirection), 0.0), shininess); // Increase the specular power for a sharper highlight
    vec3 specular = spec * light.specular * texture(texture_diffuse1, textureCoordinates).rgb;

    // attenuation for the point light
    float distance = length(light.position - fragmentPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Apply attenuation to all components (ambient, diffuse, and specular) for the point light
    // ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

	return vec3( ambient + diffuse + specular );
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
	 // ambient lighting for the spotlight
    vec3 ambient = texture(texture_diffuse1, textureCoordinates).rgb * light.ambient;

    // diffuse lighting for the spotlight 
    vec3 lightDirection = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDirection), 0.0); // Clamp the dot product between 0 and 1
    vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, textureCoordinates).rgb;

    // specular lighting for the spotlight
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDirection), 0.0), shininess); // Increase the specular power for a sharper highlight
    vec3 specular = spec * light.specular * texture(texture_diffuse1, textureCoordinates).rgb;

    // attenuation for the spotlight
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // spotlight 
    float theta = dot(lightDirection, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    attenuation *= intensity; // Apply intensity to the attenuation factor
    diffuse *= attenuation;
    specular *= attenuation;



	return ambient + diffuse + specular;
}
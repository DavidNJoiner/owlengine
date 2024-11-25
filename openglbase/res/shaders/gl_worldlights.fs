#version 330 core

#define MAX_POINT_LIGHTS 1
#define MAX_SPOT_LIGHTS 1

struct Material {
    sampler2D diffuse;  
    float shininess;    
};

struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;      
    vec3 direction;     
    float cutOff;       
    float outerCutOff;  
    float constant;     
    float linear;       
    float quadratic;    
    vec3 ambient;       
    vec3 diffuse;       
    vec3 specular;      
};

// Inputs from the vertex shader
in vec2 TexCoords;      // Interpolated texture coordinates
in vec3 Normal;         // Interpolated normal vector
in vec3 FragPos;        // Interpolated fragment position in world space
in vec3 ViewDir;        // Interpolated view direction

// Outputs to the framebuffer
out vec4 FragColor;

// Uniforms for material, lights, and camera position
uniform Material material;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

// Function to calculate point light contribution
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 ViewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(ViewDir, reflectDir), 0.0), material.shininess);
    
    // Attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    // Combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec;

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

// Function to calculate spotlight contribution
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 ViewDir)
{
    // Calculate the light direction
    vec3 lightDir = normalize(light.position - fragPos);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(ViewDir, reflectDir), 0.0), material.shininess);

    // Attenuation calculation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Spotlight intensity calculation
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // Calculate ambient, diffuse, and specular components
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec;

    // Apply attenuation and spotlight intensity
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    // Return the final lighting result
    return (ambient + diffuse + specular);
}

void main() {
    vec3 norm = normalize(Normal);
    
    vec3 finalColor = vec3(0.0);
    
    // Calculate contributions from point lights
    for(int i = 0; i < MAX_POINT_LIGHTS; i++) {
        finalColor += CalcPointLight(pointLights[i], norm, FragPos, ViewDir);
    }
    
    // Calculate contributions from spotlights
    for(int i = 0; i < MAX_SPOT_LIGHTS; i++) {
        finalColor += CalcSpotLight(spotLights[i], norm, FragPos, ViewDir);
    }
    
    // Output the final color
    FragColor = vec4(finalColor, 1.0);
}

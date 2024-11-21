#version 330 core

struct Material {
    sampler2D diffuse;  // Diffuse texture
    sampler2D specular; // Specular texture
    float shininess;    // Shininess factor for specular highlights
};

struct SpotLight {
    vec3 position;      // Light position in world space
    vec3 direction;     // Light direction
    float cutOff;       // Inner cutoff angle (cosine)
    float outerCutOff;  // Outer cutoff angle (cosine)

    float constant;     // Attenuation constant term
    float linear;       // Attenuation linear term
    float quadratic;    // Attenuation quadratic term

    vec3 ambient;       // Ambient light color
    vec3 diffuse;       // Diffuse light color
    vec3 specular;      // Specular light color
};

in vec2 TexCoords;      // Interpolated texture coordinates
in vec3 Normal;         // Interpolated normal vector
in vec3 FragPos;        // Interpolated fragment position in world space
in vec3 ViewDir;        // Interpolated view direction

out vec4 FragColor;     // Output color

uniform SpotLight spotLight; // Spotlight data
uniform Material material;   // Material properties

// Spotlight lighting calculation
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // Calculate the light direction
    vec3 lightDir = normalize(light.position - fragPos);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

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
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    // Apply attenuation and spotlight intensity
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    // Return the final lighting result
    return (ambient + diffuse + specular);
}

void main()
{
    // Normalize the normal vector
    vec3 norm = normalize(Normal);

    // Calculate the final color using the spotlight
    vec3 result = CalcSpotLight(spotLight, norm, FragPos, ViewDir);

    // Output the final fragment color
    FragColor = vec4(result, 1.0);
}

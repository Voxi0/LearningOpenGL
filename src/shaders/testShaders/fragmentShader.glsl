#version 460 core

// Structures
// Material
struct Material {
    // Lighting Maps
    sampler2D texture_diffuse1, texture_specular1;

    // Shininess
    float shininess;
};

// Light Casters
struct DirectionalLight {
    // Direction
    vec3 direction;

    // Lighting Intensity Values
    vec3 ambientIntensity, diffusionIntensity, specularIntensity;
};
struct PointLight {
    // Position
    vec3 position;

    // Lighting Intensity Values
    vec3 ambientIntensity, diffusionIntensity, specularIntensity;

    // Lighting Attenuation Values
    float attenuationConstant, attenuationLinear, attenuationQuadratic;
};
struct SpotLight {
    // Position and Direction
    vec3 position, direction;

    // Inner and Outer Cut Off
    float innerCutOff, outerCutOff;

    // Lighting Intensity Values
    vec3 ambientIntensity, diffusionIntensity, specularIntensity;

    // Lighting Attenuation Values
    float attenuationConstant, attenuationLinear, attenuationQuadratic;
};

// Lighting Calculation Functions
vec3 calculateDirectionalLight(DirectionalLight light) {
    return vec3(0.0f, 0.0f, 0.0f);
}
vec3 calculatePointLight(PointLight light, Material material, vec2 texCords, vec3 normalizedNormalVector, vec3 fragPos, vec3 viewPos) {
    // Calculate Light Attenuation Value
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.attenuationConstant + light.attenuationLinear * distance + light.attenuationQuadratic * (distance * distance));

    // Ambience
    vec3 ambience = vec3(texture(material.texture_diffuse1, texCords)) * light.ambientIntensity;

    // Diffusion
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normalizedNormalVector, lightDir), 0.0f);
    vec3 diffusion = vec3(texture(material.texture_diffuse1, texCords)) * diff * light.diffusionIntensity;

    // Specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normalizedNormalVector, halfwayDir), 0.0f), material.shininess);
    vec3 specular = vec3(texture(material.texture_specular1, texCords)) * spec * light.specularIntensity;

    // Apply Light Attenuation Value
    ambience *= attenuation;
    diffusion *= attenuation;
    specular *= attenuation;

    // Return Final Fragment Color
    return vec3(ambience + diffusion + specular);
}
vec3 calculateSpotLight(SpotLight light, Material material, vec2 texCords, vec3 normalizedNormalVector, vec3 fragPos) {
    // Calculate Light to Fragment Direction
    vec3 lightDir = normalize(light.position - fragPos);

    // Calculate Light Intensity Value
    float theta = dot(lightDir, normalize(light.direction));
    float epsilon = light.innerCutOff - light.outerCutOff;
    float lightIntensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

    // Calculate Light Attenuation Value
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.attenuationConstant + light.attenuationLinear * distance + light.attenuationQuadratic * (distance * distance));

    // Ambience
    vec3 ambience = vec3(texture(material.texture_diffuse1, texCords)) * light.ambientIntensity;

    // Diffusion
    float diff = max(dot(normalizedNormalVector, lightDir), 0.0f);
    vec3 diffusion = vec3(texture(material.texture_diffuse1, texCords)) * diff * light.diffusionIntensity;

    // Specular
    vec3 viewDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normalizedNormalVector, halfwayDir), 0.0f), material.shininess);
    vec3 specular = vec3(texture(material.texture_specular1, texCords)) * spec * light.specularIntensity;

    // Apply Light Intensity Value and Light Attenuation Value
    // Don't Apply Light Intensity Value to Ambient Value so It's Not Completely Black
    ambience *= attenuation;
    diffusion *= attenuation;
    specular *= attenuation;
    diffusion *= lightIntensity;
    specular *= lightIntensity;

    // Return Final Fragment Color
    return vec3(ambience + diffusion + specular);
}

// Input
in vec2 texCords;
in vec3 normalVec, fragPos;

// Uniform Variables
uniform Material material;
uniform SpotLight light;

// Output
out vec4 fragColor;

// Main
void main(void) {
    // Normalize Normal Vectors
    vec3 normalizedNormalVector = normalize(normalVec);

    // Calculate and Set Final Fragment Color
    fragColor = vec4(calculateSpotLight(light, material, texCords, normalizedNormalVector, fragPos), 1.0f);
}

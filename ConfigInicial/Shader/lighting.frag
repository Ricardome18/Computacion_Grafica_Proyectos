#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D texture_diffusse;
uniform Light light;
uniform Light light2;
uniform Material material;
uniform vec3 viewPos;

void main()
{
    // Calculate lighting for light
    vec3 norm = normalize(Normal);
    vec3 lightDir1 = normalize(light.position - FragPos);
    float diff1 = max(dot(norm, lightDir1), 0.0);
    vec3 reflectDir1 = reflect(-lightDir1, norm);
    vec3 viewDir = normalize(viewPos - FragPos);
    float spec1 = pow(max(dot(viewDir, reflectDir1), 0.0), 32.0);
    vec3 ambient1 = light.ambient;
    vec3 diffuse1 = light.diffuse * diff1;
    vec3 specular1 = light.specular * spec1;

    // Calculate lighting for light2
    vec3 lightDir2 = normalize(light2.position - FragPos);
    float diff2 = max(dot(norm, lightDir2), 0.0);
    vec3 reflectDir2 = reflect(-lightDir2, norm);
    float spec2 = pow(max(dot(viewDir, reflectDir2), 0.0), 32.0);
    vec3 ambient2 = light2.ambient;
    vec3 diffuse2 = light2.diffuse * diff2;
    vec3 specular2 = light2.specular * spec2;

    // Sum the results
    vec3 result = ambient1 + diffuse1 + specular1 + ambient2 + diffuse2 + specular2;
    FragColor = vec4(result, 1.0)*texture(texture_diffusse,TexCoords);;
}

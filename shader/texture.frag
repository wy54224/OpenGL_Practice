#version 330 core
struct Material{
    sampler2D diffuse[4];
    sampler2D specular[4];
    float shininess;
    int diffuseNum;
    int specularNum;
};
uniform Material material;

struct FlashLight{
    vec3 position;
    vec3 direction;
    float outerCutOff;
    float epsilon;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform FlashLight flashlight;

struct DirLight{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirlight;

struct PointLight{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};
uniform PointLight pointlight;

in vec2 TexCoord;
in vec3 midNormal;
in vec3 fragPos;
out vec4 color;
uniform vec3 viewPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 texColor, vec3 texSpec, float shininess){
    vec3 ambient = light.ambient * texColor;

    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * texColor * light.diffuse;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), shininess);
    vec3 specular = light.specular * spec * texSpec;
    return ambient + diffuse + specular;
}

vec3 CalPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 texColor, vec3 texSpec, float shininess, vec3 fragPos){
    vec3 ambient = light.ambient * texColor;

    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * texColor * light.diffuse;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), shininess);
    vec3 specular = light.specular * spec * texSpec;

    float distance = length(light.position - fragPos);
    float attentuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    return (ambient + diffuse + specular) * attentuation;
}

vec3 CalFlashLight(FlashLight light, vec3 normal, vec3 viewDir, vec3 texColor, vec3 texSpec, float shininess, vec3 fragPos){
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(-lightDir, light.direction);
    float intensity = clamp((theta - light.outerCutOff) / light.epsilon, 0.0, 1.0);
    
    vec3 ambient = light.ambient * texColor;
    ambient = vec3(0, 0, 0);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * texColor * light.diffuse;
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), shininess);
    vec3 specular = light.specular * spec * texSpec;

    diffuse *= intensity;
    specular *= intensity;
    return ambient + diffuse + specular;
}

void main(){
    vec3 viewDir = normalize(viewPos - fragPos);
    vec4 mixColor = vec4(0);
    for(int i = 0; i < material.diffuseNum; ++i)
        mixColor += texture(material.diffuse[i], TexCoord);
    vec3 texColor = vec3(mixColor);
    vec4 mixSpec = vec4(0);
    for(int i = 0; i < material.specularNum; ++i)
        mixSpec += texture(material.specular[i], TexCoord);
    vec3 texSpec = vec3(mixSpec);
    vec3 dirColor = CalcDirLight(dirlight, midNormal, viewDir, texColor, texSpec, material.shininess);
    vec3 pointColor = CalPointLight(pointlight, midNormal, viewDir, texColor, texSpec, material.shininess, fragPos);
    vec3 flashColor = CalFlashLight(flashlight, midNormal, viewDir, texColor, texSpec, material.shininess, fragPos);
    color = vec4(dirColor + pointColor + flashColor, 1.0);
}
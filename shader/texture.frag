#version 330 core
struct Material{
    sampler2D diffuse[4];
    sampler2D specular[4];
    float shininess;
    int diffuseNum;
    int specularNum;
};
uniform Material material;

struct DirLight{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirlight;

in vec2 TexCoord;
in vec3 midNormal;
in vec3 fragPos;
in vec4 fragPosLightSpace;
out vec4 color;
uniform vec3 viewPos;
uniform sampler2D shadow;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 direction, vec2 pixelSize){
	vec3 projCoord = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoord = projCoord * 0.5 + 0.5;
	float currentDepth = projCoord.z > 1 ? 0 : projCoord.z;
	float bias = max(0.05 * (1 - dot(normal, direction)), 0.005);
	float shadowCal = 0;
	for(int x = -1; x <= 1; ++x){
		for(int y = -1; y <= 1; ++y){
			float closetDepth = texture(shadow, projCoord.xy + vec2(x, y) * pixelSize).r;
			shadowCal += currentDepth - bias > closetDepth ? 1 : 0;
		}
	}
	return shadowCal / 9.0;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 texColor, vec3 texSpec, float shininess){
    vec3 ambient = light.ambient * texColor;

    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * texColor * light.diffuse;

    vec3 reflectDir = normalize(reflect(-lightDir, normal));
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), shininess);
    vec3 specular = light.specular * spec * texSpec;
    return ambient + (diffuse + specular) * (1 - ShadowCalculation(fragPosLightSpace, normal, light.direction,  1.0 / textureSize(shadow, 0)));
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
    color = vec4(dirColor, 1);
}

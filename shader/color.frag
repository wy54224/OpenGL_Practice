#version 330 core
in vec3 midColor;
in vec3 midNormal;
in vec3 fragPos;
in vec4 fragPosLightSpace;
out vec4 color;
uniform float Ka, Kd, Ks, Shininess;
uniform vec3 direction;
uniform vec3 viewPos;
uniform vec3 lightColor;
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

void main(){
    vec3 viewDir = normalize(viewPos - fragPos);
    float ambient = Ka;
    vec3 lightDir = normalize(-direction);
    float diff = max(dot(lightDir, midNormal), 0.0);
    float diffuse = Kd * diff;

    vec3 reflectDir = reflect(-lightDir, midNormal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), Shininess);
    float specular = Ks * spec;
    color = vec4((ambient + (diffuse + specular) * (1 - ShadowCalculation(fragPosLightSpace, midNormal, direction, 1.0 / textureSize(shadow, 0)))) * lightColor * midColor, 1.0f);
}

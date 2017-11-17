#version 330 core
in vec3 midColor;
in vec3 midNormal;
in vec3 fragPos;
out vec4 color;

uniform float Ka, Kd, Ks, Shininess;
uniform vec3 direction;
uniform vec3 viewPos;
uniform vec3 lightColor;

void main(){
    vec3 viewDir = normalize(viewPos - fragPos);

    float ambient = Ka;

    vec3 lightDir = normalize(-direction);
    float diff = max(dot(lightDir, midNormal), 0.0);
    float diffuse = Kd * diff;

    vec3 reflectDir = reflect(-lightDir, midNormal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), Shininess);
    float specular = Ks * spec;
    color = vec4((ambient + diffuse + specular) * lightColor * midColor, 1.0f);
}
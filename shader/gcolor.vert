#version 330 core
layout(location = 0)in vec3 pos;
layout(location = 1)in vec3 color;
layout(location = 2)in vec3 normal;

out vec3 midColor;

uniform mat4 projection;
uniform vec4 view;
uniform mat4 model;
uniform vec3 viewPos;

uniform float Ka, Kd, Ks, Shininess;
uniform vec3 direction;
uniform vec3 lightColor;

vec4 multiQuat(vec4 v1, vec4 v2)
{
	vec4 v;
	v.w = v1.w * v2.w - v1.x * v2.x - v1.y * v2.y - v1.z * v2.z;
	v.x = v1.w * v2.x + v2.w * v1.x + v1.y * v2.z - v1.z * v2.y;
	v.y = v1.w * v2.y + v2.w * v1.y + v1.z * v2.x - v1.x * v2.z;
	v.z = v1.w * v2.z + v2.w * v1.z + v1.x * v2.y - v1.y * v2.x;
	return v;
}

void main(){
    vec4 tmpPos = model * vec4(pos, 1.0f);
    gl_Position =tmpPos -  vec4(viewPos, 0);
    vec4 _view = -view;
    _view.w = -_view.w;
    gl_Position = multiQuat(multiQuat(view, gl_Position), _view);
    gl_Position = projection * gl_Position;
    vec3 midNormal = mat3(transpose(inverse(model))) * normal;

    vec3 viewDir = normalize(viewPos - vec3(tmpPos));

    float ambient = Ka;

    vec3 lightDir = normalize(-direction);
    float diff = max(dot(lightDir, midNormal), 0.0);
    float diffuse = Kd * diff;

    vec3 reflectDir = reflect(-lightDir, midNormal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), Shininess);
    float specular = Ks * spec;

    midColor = (ambient + diffuse + specular) * lightColor * color;
}
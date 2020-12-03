#version 330 core
in vec3 fragPosition;
in vec3 normal;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform bool isSun;

void main()
{
    float kd = 0.8;
    vec3 n = normalize(normal);
    vec3 l = normalize(lightPosition - fragPosition);

    float diff = max(dot(n,l), 0.0);
    vec3 diffuse = kd * diff * lightColor;

    float ks = 1.0;
    vec3 v = normalize(cameraPosition - fragPosition);
    vec3 r = reflect(-l, n);

    float spec = pow(max(dot(v, r), 0.0), 3.0);
    vec3 specular = ks * spec * lightColor;

    vec3 light = (diffuse + specular) * objectColor;
    if (isSun) {
        FragColor = vec4(objectColor, 1.0);
    } else {
        FragColor = vec4(light, 1.0);
    }
}
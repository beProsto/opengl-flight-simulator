#version 330 core

const vec3 cLightDir = normalize(vec3(1.0f, 1.0f, 1.0f));
const vec3 cColor = vec3(0.6, 0.6, 0.5);
in vec3 vNormal;

out vec4 FragColor;

void main() {
    float lightTint = (dot(vNormal, cLightDir) * 0.5 + 0.5) * 0.4;
    FragColor = vec4(cColor + lightTint, 1.0);
}
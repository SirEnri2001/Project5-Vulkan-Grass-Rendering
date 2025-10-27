#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform CameraBufferObject {
    mat4 view;
    mat4 proj;
} camera;

// TODO: Declare fragment shader inputs

layout(location = 0) in vec4 inWorldPos;
layout(location = 1) in vec4 inNormal;
layout(location = 2) in vec4 inUV;

layout(location = 0) out vec4 outColor;

void main() {
    // TODO: Compute fragment color
    vec3 lightDir = normalize(vec3(1.0, -1.0, 1.0));
    vec3 grassColor = vec3(0.2, 0.8, 0.2);
    float lightIntensity = max(dot(normalize(inNormal.xyz), lightDir), 0.0);
    outColor = vec4(inNormal.xyz * 0.5 + 0.5, 1.);
}

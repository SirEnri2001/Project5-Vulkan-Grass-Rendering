#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform CameraBufferObject {
    mat4 view;
    mat4 proj;
	vec4 viewDir;
	vec4 viewPos;
} camera;

// TODO: Declare fragment shader inputs

layout(location = 0) in vec4 inWorldPos;
layout(location = 1) in vec4 inNormal;
layout(location = 2) in vec4 inUV;
layout(location = 3) in vec4 inAlpha;
layout(location = 4) in vec4 inTangent;
layout(location = 5) in vec4 inCotangent;

layout(location = 0) out vec4 outColor;

void main() {
    // TODO: Compute fragment color
    vec3 normal = normalize(inNormal.xyz);
    vec3 lightDir = normalize(vec3(1.0, -1.0, 1.0));

    if(dot(normal, camera.viewDir.xyz) > 0.0){
        normal = -normal;
    }

    vec3 grassColor = vec3(0.2, 0.8, 0.2);
    float lightIntensity = abs(dot(normalize(normal), lightDir))*0.5+0.5;
    outColor = vec4(lightIntensity * grassColor, 1.);
}

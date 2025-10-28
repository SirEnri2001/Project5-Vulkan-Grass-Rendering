#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(quads, equal_spacing, ccw) in;

layout(set = 0, binding = 0) uniform CameraBufferObject {
    mat4 view;
    mat4 proj;
} camera;

// TODO: Declare tessellation evaluation shader inputs and outputs
layout(location = 0) in vec4 inPosition_Direction[];
layout(location = 1) in vec4 inBezier_Height[];
layout(location = 2) in vec4 inModelGuide_Width[];
layout(location = 3) in vec4 inUpVec_Stiffness[];


layout(location = 0) out vec4 outWorldPos;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out vec4 outUV;
layout(location = 3) out vec4 outAlpha;
layout(location = 4) out vec4 outTangent;
layout(location = 5) out vec4 outCotangent;

void main() {
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
    float height = inBezier_Height[0].w;
    float width = inModelGuide_Width[0].w;
    float directionRad = inPosition_Direction[0].w;

    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3));
    vec3 grassBaseColor = vec3(0.1, 0.6, 0.1);

    vec3 v0 = inPosition_Direction[0].xyz;
    vec3 v1 = inBezier_Height[0].xyz;
    vec3 v2 = inModelGuide_Width[0].xyz;

    vec3 a = mix(v0, v1, v);
    vec3 b = mix(v1, v2, v);
    vec3 c = mix(a, b, v);
    vec3 t1 = vec3(sin(directionRad), 0., cos(directionRad));
    outCotangent = vec4(t1, 0.);
    vec3 c0 = c - width * t1 * 0.5;
    vec3 c1 = c + width * t1 * 0.5;
    vec3 t0 = normalize(b - a);
    outTangent = vec4(t0, 0.);
    vec3 normal = normalize(cross(t0, t1));
    float t = u + 0.5*v - u * v;
    outWorldPos = vec4(mix(c0, c1, t), 1.);


	// TODO: Use u and v to parameterize along the grass blade and output positions for each vertex of the grass blade
    outUV = vec4(u, v, 0., 1.);
    outNormal = vec4(normal, 0.);
	gl_Position = camera.proj * camera.view * outWorldPos;
    /*
	vec3 grassViewDir = normalize(blade.v0.xyz - camera.viewPos.xyz);
	if(abs(dot(t1, grassViewDir))>0.98){
		return;
	}*/
    outAlpha.x = 0.5;
}

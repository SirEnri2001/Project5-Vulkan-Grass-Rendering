#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(vertices = 1) out;

layout(set = 0, binding = 0) uniform CameraBufferObject {
    mat4 view;
    mat4 proj;
	vec4 viewDir;
	vec4 viewPos;
} camera;


layout(set = 2, binding = 0) uniform Params {
	vec4 windDirection;
	vec4 lod;
	float windNoiseScale;
	float windStrength;
	float gravityStrength;
	float cullingScale;
	float cullingDistance;
} params;

// TODO: Declare tessellation control shader inputs and outputs
layout(location = 0) in vec4 inPosition_Direction[];
layout(location = 1) in vec4 inBezier_Height[];
layout(location = 2) in vec4 inModelGuide_Width[];
layout(location = 3) in vec4 inUpVec_Stiffness[];


layout(location = 0) out vec4 outPosition_Direction[];
layout(location = 1) out vec4 outBezier_Height[];
layout(location = 2) out vec4 outModelGuide_Width[];
layout(location = 3) out vec4 outUpVec_Stiffness[];

void main() {
	// Don't move the origin location of the patch
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

	// TODO: Write any shader outputs
    outPosition_Direction[gl_InvocationID] = inPosition_Direction[gl_InvocationID];
    outBezier_Height[gl_InvocationID] = inBezier_Height[gl_InvocationID];
    outModelGuide_Width[gl_InvocationID] = inModelGuide_Width[gl_InvocationID];
    outUpVec_Stiffness[gl_InvocationID] = inUpVec_Stiffness[gl_InvocationID];
	vec3 v0 = inPosition_Direction[0].xyz;
	// TODO: Set level of tesselation
	float d_proj = length(camera.viewPos.xyz - v0);
	float tessLevel = 6.0;
	if(d_proj > params.lod.x){
		tessLevel = 4.0;
	}
	if(d_proj > params.lod.y){
		tessLevel = 2.0;
	}
	if(d_proj > params.lod.z){
		tessLevel = 1.0;
	}
	gl_TessLevelInner[0] = tessLevel;
	gl_TessLevelInner[1] = tessLevel;
	gl_TessLevelOuter[0] = tessLevel;
	gl_TessLevelOuter[1] = tessLevel;
	gl_TessLevelOuter[2] = tessLevel;
	gl_TessLevelOuter[3] = tessLevel;
	
}

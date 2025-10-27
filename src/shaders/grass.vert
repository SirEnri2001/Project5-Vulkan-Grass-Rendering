
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 1, binding = 0) uniform ModelBufferObject {
    mat4 model;
};

// TODO: Declare vertex shader inputs and outputs
layout(location = 0) in vec4 inPosition_Direction;
layout(location = 1) in vec4 inBezier_Height;
layout(location = 2) in vec4 inModelGuide_Width;
layout(location = 3) in vec4 inUpVec_Stiffness;


layout(location = 0) out vec4 outPosition_Direction;
layout(location = 1) out vec4 outBezier_Height;
layout(location = 2) out vec4 outModelGuide_Width;
layout(location = 3) out vec4 outUpVec_Stiffness;

out gl_PerVertex {
        vec4 gl_Position;
        float gl_PointSize;
        float gl_ClipDistance[];
        float gl_CullDistance[];
};

void main() {
	// TODO: Write gl_Position and any other shader outputs
    vec4 inPos = inPosition_Direction;
    inPos.w = 1.;
    gl_Position =  inPos;
    outPosition_Direction = inPosition_Direction;
    outBezier_Height = inBezier_Height;
    outModelGuide_Width = inModelGuide_Width;
    outUpVec_Stiffness = inUpVec_Stiffness;
}

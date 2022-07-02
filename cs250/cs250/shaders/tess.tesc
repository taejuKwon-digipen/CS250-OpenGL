#version 460

layout(vertices = 3) out;

in vec3 vPosition[];

out vec3 tcPosition[];

uniform float TessLevelInner;
uniform float TessLevelOuter;

void main()
{
	gl_TessLevelInner[0] = TessLevelInner;

	gl_TessLevelOuter[0] = TessLevelOuter;
	gl_TessLevelOuter[1] = TessLevelOuter;
	gl_TessLevelOuter[2] = TessLevelOuter;

	tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];

}
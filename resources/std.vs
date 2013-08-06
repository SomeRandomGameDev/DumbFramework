#version 400 core  

precision highp float;
precision highp int;

layout(location = 0) in vec4 iPosition;

uniform mat4 g_ModelviewProjection;

void main()
{
	gl_Position = g_ModelviewProjection * iPosition;
}
